#include "globals.h"
#include "gpu_sift.h"
#include "tracker.h"
#include "golfball.h"
#include "classifier.h"
#include "ids_camera.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <cstddef>


// Set of all global variables (Interface)
namespace golfGlobals
{
	bool dataPipe = false; // Tell us if we are connected to the interface
	bool dataPipeInitialized = false; 
	HANDLE filePipe = NULL; // pipe between Interface and application (Images, Stats, etc. diplayed on the interface)
	HANDLE mutexPipe = NULL; // handle to lock the pipe while is being used
	golfTransmission golfData; // Class that holds all the options
	int bigBallSize; // Maximum size of a ball in all images (ball size from closer camera)

	golfBallTracker* tracker; // pointer to the Tracker
	Gpu_Sift* extractor; // pointer to the GPU-SIFT extractor

	// set of offsets and images sizes used by the interface options and controls
	namespace golfTransmissionConsts 
	{
		// image related sizes
		const int maximgwidth = 2048;
		const int maximgheight = 1088;
		//offsets of the image pointers wrt dataPipe
		const int imgsOffset1 = sizeof(golfTransmission);
		const int imgsOffset2 = imgsOffset1 + maximgwidth*maximgheight*3;
		const int imgsOffset3 = imgsOffset2 + maximgwidth*maximgheight*3;
		const int imgsOffset4 = imgsOffset3 + maximgwidth*maximgheight*3; // New ball case (for emailing)
		const int fileSize = imgsOffset4 + maximgwidth*maximgheight*3; // total size of the map memory file 
		// offsets for the options
		const int optionsOffset = offsetof(golfTransmission,sift_thresh);
		const int optionsSize = imgsOffset1-optionsOffset;
		// offsets for the controls
		const int controlsOffset = offsetof(golfTransmission,render_img1);
		const int controlsSize = optionsOffset-controlsOffset;
		// image sizes - roi's for each camera
		const int imgsizeoffset = offsetof(golfTransmission, img_width1);
		const int imgsizesize = optionsOffset-imgsizeoffset;
	}
}

// prints windows errors in string form
void printLastError()
{
	char* buf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				    NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&buf, 0, NULL);
	std::cout << buf << std::endl;
	std::cout.flush();
	LocalFree(buf);
}

void cleanup()
{
	golfGlobals::tracker->stopCameras();
}

BrandDecisionRule golfGlobals::DecisionRule = BrandDecisionRule();
int golfGlobals::max_queue_size = 100;
cv::Mat golfGlobals::templateImg;
std::vector<std::vector<cv::Point> > golfGlobals::templateContours;


int main( int argc, char* argv[] )
{	
	HANDLE hThread = GetCurrentThread();
	SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
	//1 - Linking program to the interface
	if(argc==3)
	{
		// opens the map memory file
		golfGlobals::filePipe = OpenFileMapping(FILE_MAP_WRITE, false, argv[1]);
		if(golfGlobals::filePipe==NULL)
		{
			std::cout << "Error opening mapped file." << std::endl;
			printLastError();
			goto errorMMFile;
		}
		// opens the mutex (lock)
		golfGlobals::mutexPipe = CreateMutex(NULL, false, argv[2]);
		if(golfGlobals::mutexPipe==NULL)
		{
			std::cout << "Error opening mutex." << std::endl;
			printLastError();
			goto errorMMFile;
		}
		golfGlobals::dataPipe = true;
	}
	else
	{
		errorMMFile:;
		std::cout << "This executable is meant to run with a memory mapped file to an interface.\nPress Enter to continue without the interface...\n";
		getchar();
	}
	
	//2 - Initialization
	Gpu_Sift SiftExtractor; // SIFT extractor
	golfBallTracker GolfBallTracker; // Golfball tracker
	if(golfBall::initialize())
		return E_FAIL;
	if(GolfBallTracker.initialize())
		return E_FAIL;
	if(golfGlobals::dataPipe)
		while(!GolfBallTracker.updateTransmission());
	if(classifyJob::initialize())
		return E_FAIL;
	// Copying pointers of tracker and SIFT so they are visible for all threads
	golfGlobals::tracker = &GolfBallTracker;
	golfGlobals::extractor = &SiftExtractor;
	// Classfiers	
	std::vector<classifyJobNB*> sift_balls; // brand classification module
	std::vector<classifyJobCC*> balls;  // model classification module
	std::vector<classifyJobNike*> nike_balls;  // Nike classification module
	if (golfGlobals::golfData.sort_brands)
	{

		if(SiftExtractor.initialize())
			return E_FAIL;
		while(true)
		{
			GolfBallTracker.update();
			GolfBallTracker.getBallImages(sift_balls, nike_balls);
			//unlocking current frame
			GolfBallTracker.releaseFrame(); 

			golfBall::addJobsNike(nike_balls);

			// Computation of SIFT Images (empty in case of model recognition module)
			SiftExtractor.upload(sift_balls);
			if (SiftExtractor.download(sift_balls))
			{
				// Processing for brand module -> Add jobs to new threads (empty in case of model recognition module)
				golfBall::addJobsNB(sift_balls);
			}
			sift_balls.clear();
			nike_balls.clear();
		}	
	}
	else 
		if (golfGlobals::golfData.sort_models)
		{

			while(true)
			{
				GolfBallTracker.update();
				GolfBallTracker.getBallImages(balls);
				//unlocking current frame
				GolfBallTracker.releaseFrame(); 
				// Processing for model module -> Add jobs to new threads (empty in case of brand recognition module)
				golfBall::addJobsCC(balls);
				balls.clear();
			}
			
		
		}
}
