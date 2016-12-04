#include "tracker.h"
#include "golfball.h"
#include "ids_camera.h"
#include "classifier.h"
#include "gpu_sift.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max
#include <iostream>
#include <fstream>

golfBallTracker::golfBallTracker(): 
	entrance(0,0,0,0),
	fontFace(cv::FONT_HERSHEY_SCRIPT_SIMPLEX),
	fontScale(1.0),
	fontThickness(2),
	barMin(30, 0, 0),
	barMax(255, 40, 40),
	frameData1(NULL),
	frameData2(NULL),
	frameData3(NULL),
	found_aoi1(false),
	found_aoi2(false),
	found_aoi3(false)
{
}

int golfBallTracker::initialize()
{
	idsCam1 = new IDS_Camera;
	idsCam2 = new IDS_Camera;
	idsCam3 = new IDS_Camera;
	
	//idsCam1->setCaptureFiles("C:\\Videos\\Video1_good\\cam1_images", 2000);
	//idsCam2->setCaptureFiles("C:\\Videos\\Video1_good\\cam2_images", 2000);
	//idsCam3->setCaptureFiles("C:\\Videos\\Video1_good\\cam3_images", 2000);

	std::cout << "Initializing cameras... " << std::endl;

	if(!idsCam1->initiCamera(1, "4102761785"))
	{
		std::cout << "Couldn't initialize camera 1\n";
		getchar();
		return -1;
	}

	if(!idsCam2->initiCamera(2, "4102759796"))
	{
		std::cout << "Couldn't initialize camera 2\n";
		getchar();
		return -1;
	}

	if(!idsCam3->initiCamera(3, "4102759795"))
	{
		std::cout << "Couldn't initialize camera 3\n";
		getchar();
		return -1;
	}

	idsCam1->setinifile("paramsCam_Cam1_Nov.ini");
	idsCam2->setinifile("paramsCam_Cam2_Nov.ini");
	idsCam3->setinifile("paramsCam_Cam3_Nov.ini");

	idsCam1->startCapture();
	idsCam2->startCapture();
	idsCam3->startCapture();
	
	Frame1 = cv::Mat(idsCam1->m_nMaxSizeY, idsCam1->m_nMaxSizeX, CV_8UC3, frameData1);
	Frame2 = cv::Mat(idsCam2->m_nMaxSizeY, idsCam2->m_nMaxSizeX, CV_8UC3, frameData2);
	Frame3 = cv::Mat(idsCam3->m_nMaxSizeY, idsCam3->m_nMaxSizeX, CV_8UC3, frameData3);
	std::cout << "Done! " << std::endl;

	return 0;
}

void golfBallTracker::trackBalls()
{
	for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end();)
	{
		golfBall* ball = *ballit;
		
		cv::Rect window = ball->trackWindow;
		window.width = window.width*7/3;
		window.x -= window.width/2;
		window.y = entrance.y + entrance.height/2;
		window.height = 1;
		window &= cv::Rect(cv::Point(0,0), Frame1.size());
		if(window.area()==0)
		{
			ball->ballClean();
			goto deleteBall;
		}

		{
			cv::Mat mask;
			cv::inRange(Frame1(window), barMin, barMax, mask);

			int mean = 0, count = 0;
			for(int i = 0; i<mask.cols; ++i)
			{
				if(mask.at<uchar>(i))
				{
					mean += i;
					++count;
				}
			}
			if(count==0)
			{
				ball->ballClean();
				goto deleteBall;
			}
			mean /= count;

			window = cv::Rect(mean+window.x-count/2, entrance.y, count, entrance.height);
		}

		ball->trackWindow = window;
		

		//Check if the ball is at the edge of the screen.
		if(ball->isAtImageEdge(golfGlobals::golfData.ballsize/2))//Frame1.cols))
		{
			ball->ballFinished();
		//	ball->ballFinished2ndVer();
			receivedBrands.push_back(ball->brand);
			goto deleteBall;
		}

		//Check if the tracking jumped to a previous bar.
		if(ballit!=trackedBalls.begin())
		{
			golfBall* ball2 = *(ballit-1);
			//if(std::abs(ball->trackWindow.x-ball2->trackWindow.x)<golfGlobals::golfData.ballsize/2)
			if(ball2->trackWindow.x-ball->trackWindow.x<golfGlobals::golfData.ballsize) // Ball is the left most ball, ball2 is the second ball entered
			{
				ball->ballClean();
				goto deleteBall;
			}
		}

		++ballit;
		continue;
		deleteBall:;
			ballit = trackedBalls.erase(ballit);
	}
}

void golfBallTracker::searchForBars()
{
	if(trackedBalls.size()>0)
	{
		golfBall* ball = trackedBalls.front();
		//if(std::abs(ball->trackWindow.x - entrance.x)<golfGlobals::golfData.ballsize)
		if((entrance.x - ball->trackWindow.x)<golfGlobals::golfData.ballsize)//Changed to go from right to left
			return;
	}
		
	cv::Rect window(entrance.x, entrance.y+entrance.height/2, entrance.width, 1);
	cv::Mat bar;
	cv::inRange(Frame1(window), barMin, barMax, bar);
	int numPoints = cv::countNonZero(bar);

	if(numPoints > window.width*2/3)
	{
		golfBall* ball = new golfBall(cv::Rect(entrance.x,entrance.y,entrance.width,entrance.height));
		trackedBalls.insert(trackedBalls.begin(), ball);
	}
}

void golfBallTracker::stopCameras()
{
	if(idsCam1) idsCam1->Cleanup();
	if(idsCam2) idsCam2->Cleanup();
	if(idsCam3) idsCam3->Cleanup();
}

static void getGreen(cv::Mat& in, cv::Mat& out)
{
	uchar* o = out.data;
	for(int i = 0, e = in.cols*in.rows; i<e; ++i, ++o)
		*o = in.at<cv::Vec3b>(i)[1];
}
#define maxSiftFrames 55
void golfBallTracker::getBallImages(std::vector<classifyJobNB*>& sift_balls,  std::vector<classifyJobNike*>& balls)
{
	for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end(); ++ballit)
	{
		golfBall* ball = *ballit;
		bool classifyNike = ball->templatevotes[3] < 4;
		if(!ball->identified && ball->emptySlotsCount < 10)
		{
			int isEmptySlot = 1;
			if(frameData1)
			{
				cv::Rect roi = ball->getBallROI(1, 1);
				roi &= cv::Rect(cv::Point(0,0),Frame1.size());
				if(roi.width==roi.height && roi.area()>0)
				{
					cv::Mat frame(roi.height, roi.width, CV_8UC1), imgThresh;
					std::vector<classifyJob::component> compenents;
					getGreen(Frame1(roi), frame);
					int pixels = classifyJob::thresholdImage(1, frame, imgThresh, isEmptySlot, ball);
					if(ball->observed_times < maxSiftFrames && pixels > 1000 && pixels < 3000 && golfGlobals::golfData.sort_brands)
					{
						classifyJobNB* job = new classifyJobNB(ball, 1, frame, imgThresh);
						sift_balls.push_back(job);
					}
					if(pixels < 4000 && pixels > 500 && classifyNike)
					{
						classifyJob::findComponents(1, frame, imgThresh, compenents);
						if(compenents.size()>0)
						{
							classifyJobNike* job = new classifyJobNike(ball, 1, frame, imgThresh);
							job->components.swap(compenents);
							balls.push_back(job);
						}
					}
				}
			}

			if(frameData2)
			{
				cv::Rect roi = ball->getBallROI(1, 2);
				roi &= cv::Rect(cv::Point(0,0),Frame2.size());
				if(roi.width==roi.height && roi.area()>0)
				{
					cv::Mat frame(roi.height, roi.width, CV_8UC1), imgThresh;
					std::vector<classifyJob::component> compenents;
					getGreen(Frame2(roi), frame);
					int pixels = classifyJob::thresholdImage(2, frame, imgThresh, isEmptySlot, ball);
					if(ball->observed_times < maxSiftFrames && pixels > 1000 && pixels < 3000 && golfGlobals::golfData.sort_brands)
					{
						classifyJobNB* job = new classifyJobNB(ball, 2, frame, imgThresh);
						sift_balls.push_back(job);
					}
					if(pixels < 4000 && pixels > 500 && golfGlobals::golfData.sort_brands && classifyNike)
					{
						classifyJob::findComponents(2, frame, imgThresh, compenents);
						if(compenents.size()>0)
						{
							classifyJobNike* job = new classifyJobNike(ball, 2, frame, imgThresh);
							job->components.swap(compenents);
							balls.push_back(job);
						}
					}
				}
			}

			if(frameData3)
			{
				cv::Rect roi = ball->getBallROI(1, 3);
				roi &= cv::Rect(cv::Point(0,0),Frame3.size());
				if(roi.width==roi.height && roi.area()>0)
				{
					cv::Mat frame(roi.height, roi.width, CV_8UC1), imgThresh;
					std::vector<classifyJob::component> compenents;
					getGreen(Frame3(roi), frame);
					int pixels = classifyJob::thresholdImage(3, frame, imgThresh, isEmptySlot, ball);
					//int pixels = cv::countNonZero(imgThresh);
					if(ball->observed_times < maxSiftFrames && pixels > 1000 && pixels < 5000 && golfGlobals::golfData.sort_brands)
					{
						classifyJobNB* job = new classifyJobNB(ball, 3, frame, imgThresh);
						sift_balls.push_back(job);
					}
					if(pixels < 4000 && pixels > 800 && golfGlobals::golfData.sort_brands && classifyNike)
					{
						classifyJob::findComponents(3, frame, imgThresh, compenents);
						if(compenents.size()>0)
						{
							classifyJobNike* job = new classifyJobNike(ball, 3, frame, imgThresh);
							job->components.swap(compenents);
							balls.push_back(job);
						}
					}
				}
			}
		}			
	}
}

void golfBallTracker::getBallImages(std::vector<classifyJobCC*>& balls)
{
	for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end(); ++ballit)
	{
		golfBall* ball = *ballit;
		if(!ball->identified && ball->emptySlotsCount < 10)
		{
			int isEmptySlot = 1;
			if(frameData1)
			{
				cv::Rect roi = ball->getBallROI(1, 1);
				roi &= cv::Rect(cv::Point(0,0),Frame1.size());
				if(roi.width==roi.height && roi.area()>0)
				{
					cv::Mat frame(roi.height, roi.width, CV_8UC1), imgThresh;
					std::vector<classifyJob::component> compenents;
					getGreen(Frame1(roi), frame);
					int pixels = classifyJob::thresholdImage(1, frame, imgThresh, isEmptySlot, ball);
					if(pixels < 6000 && pixels > 200) // first threshold over the number of black pixels
					{
						classifyJob::findComponents(1, frame, imgThresh, compenents);
						if(compenents.size()>0)
						{
								classifyJobCC* job = new classifyJobCC(ball, 1, frame, imgThresh);
								job->components.swap(compenents);
								balls.push_back(job);
						}
					}
				}
			}

			if(frameData2)
			{
				cv::Rect roi = ball->getBallROI(1, 2);
				roi &= cv::Rect(cv::Point(0,0),Frame2.size());
				if(roi.width==roi.height && roi.area()>0)
				{
					cv::Mat frame(roi.height, roi.width, CV_8UC1), imgThresh;
					std::vector<classifyJob::component> compenents;
					getGreen(Frame2(roi), frame);
					//int pixels = classifyJob::thresholdImage(2, frame, imgThresh);
					int pixels = classifyJob::thresholdImage(2, frame, imgThresh, isEmptySlot, ball);
					if(pixels < 6000 && pixels > 200)
					{
						classifyJob::findComponents(2, frame, imgThresh, compenents);
						if(compenents.size()>0)
						{
								classifyJobCC* job = new classifyJobCC(ball, 2, frame, imgThresh);
								job->components.swap(compenents);
								balls.push_back(job);
						}
					}
				}
			}

			if(frameData3)
			{
				cv::Rect roi = ball->getBallROI(1, 3);
				roi &= cv::Rect(cv::Point(0,0),Frame3.size());
				if(roi.width==roi.height && roi.area()>0)
				{
					cv::Mat frame(roi.height, roi.width, CV_8UC1), imgThresh;
					std::vector<classifyJob::component> compenents;
					getGreen(Frame3(roi), frame);
					int pixels = classifyJob::thresholdImage(2, frame, imgThresh, isEmptySlot, ball);
					if(pixels < 6000 && pixels > 200)
					{
						classifyJob::findComponents(3, frame, imgThresh, compenents);
						if(compenents.size()>0)
						{
								classifyJobCC* job = new classifyJobCC(ball, 3, frame, imgThresh);
								job->components.swap(compenents);
								balls.push_back(job);
						}
					}
				}
			}
		}
	}
}

golfBall* golfBallTracker::findBall(int id)
{
	for(std::vector<golfBall*>::iterator ball = trackedBalls.begin(); ball!=trackedBalls.end(); ++ball){
		if((*ball)->id==id)
			return *ball;
	}
	return NULL;
}

void golfBallTracker::downSample(cv::Mat& in, cv::Mat& out)
{
	cv::resize(in, out, cv::Size(), 0.25, 0.25, CV_INTER_NN);
}

bool golfBallTracker::findAOI(cv::Mat& frame, IDS_Camera* camera)
{
	cv::Mat mask;
	cv::inRange(frame, barMin, barMax, mask);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5), cv::Point(-1,-1));
	int ero_iter = 5;
	cv::Mat imgEroded;
	cv::erode(mask, imgEroded, kernel, cv::Point(-1,-1), ero_iter);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(imgEroded, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	int maxarea = 0;
	cv::Rect maxrect;
	for(std::vector<std::vector<cv::Point>>::iterator i = contours.begin(); i!=contours.end(); ++i)
	{
		cv::Rect rect = cv::boundingRect(*i);
		int area = rect.area();
		if(area > maxarea)
		{
			maxarea = area;
			maxrect = rect;
		}
	}

	if(std::abs(maxrect.height-golfGlobals::golfData.ballsize)<150 && std::abs(maxrect.width*2-golfGlobals::golfData.ballsize)<150)
	{
		cv::Rect imRect = cv::Rect(cv::Point(0,0), frame.size());
		cv::Rect newAOI = cv::Rect(0, maxrect.y-20, frame.cols, maxrect.height+200);
		if(camera == idsCam3)
			newAOI.y -= 100;
		newAOI &= imRect;

		if(camera == idsCam1)
		{
			entrance = maxrect;
			entrance.x = 150;
			//entrance.y -= newAOI.y;
			entrance &= imRect;
		}

		return true;
	}
	return false;
}

void golfBallTracker::update()
{
	frameData1 = idsCam1->freezeFrame();
	frameData2 = idsCam2->freezeFrame();
	frameData3 = idsCam3->freezeFrame();

	Frame1.data = (unsigned char*)frameData1;
	Frame2.data = (unsigned char*)frameData2;
	Frame3.data = (unsigned char*)frameData3;

	if(frameData1)
	{
		if(entrance.area()!=0)
		{
			searchForBars();
			trackBalls();
		}
		
		//#define SaveTrackedBoxes
		#if defined(SaveTrackedBoxes)
			static std::ofstream boxfile1("boxes1.txt");
			static std::ofstream boxfile2("boxes2.txt");
			static std::ofstream boxfile3("boxes3.txt");
			static int frameid = 0;
			for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end(); ++ballit)
			{
				golfBall* ball = *ballit;
				{
					cv::Rect roi = ball->getBallROI(1, 1);
					boxfile1 << frameid << "," << ball->id << "," << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << std::endl;
				}
				{
					cv::Rect roi = ball->getBallROI(1, 2);
					boxfile2 << frameid << "," << ball->id << "," << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << std::endl;
				}
				{
					cv::Rect roi = ball->getBallROI(1, 3);
					boxfile3 << frameid << "," << ball->id << "," << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << std::endl;
				}
			}
			++frameid;
		#endif
	}

	if(golfGlobals::dataPipe)
		updateTransmission();
}

void golfBallTracker::releaseFrame()
{
	idsCam1->unlockImage(frameData1);
	idsCam2->unlockImage(frameData2);
	idsCam3->unlockImage(frameData3);
}

bool golfBallTracker::updateTransmission()
{
	if(WaitForSingleObject(golfGlobals::mutexPipe, 2)!=WAIT_OBJECT_0) return false;
	//Update options
	void* data;

	data = MapViewOfFile(golfGlobals::filePipe, FILE_MAP_WRITE, 0, 0, golfGlobals::golfTransmissionConsts::fileSize);
	if(data==NULL)
	{
		printLastError();
		ReleaseMutex(golfGlobals::mutexPipe);
		return false;
	}

	if(((char*)data)[offsetof(golfGlobals::golfTransmission,controlsClk)])
	{
		((char*)data)[offsetof(golfGlobals::golfTransmission,controlsClk)] = 0;
		memcpy((char*)&golfGlobals::golfData + golfGlobals::golfTransmissionConsts::controlsOffset,(char*)data+golfGlobals::golfTransmissionConsts::controlsOffset,golfGlobals::golfTransmissionConsts::controlsSize);
	}

	if(((char*)data)[offsetof(golfGlobals::golfTransmission,optionsClk)])
	{
		((char*)data)[offsetof(golfGlobals::golfTransmission,optionsClk)] = 0;
		
		golfGlobals::golfData.sift_thresh = *(double*)((char*)data+offsetof(golfGlobals::golfTransmission,sift_thresh));
		golfGlobals::golfData.max_compenents = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,max_compenents));
		
		int newRoiy = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,roi_y));
		int newRoih = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,roi_height));
		int newRoiy2 = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,roi_y2));
		int newRoih2 = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,roi_height2));
		int newRoiy3 = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,roi_y3));
		int newRoih3 = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,roi_height3));
		if(newRoiy!=golfGlobals::golfData.roi_y || newRoih!=golfGlobals::golfData.roi_height)
		{
			golfGlobals::golfData.roi_y = newRoiy;
			golfGlobals::golfData.roi_height = newRoih;
			idsCam1->setROI(0, golfGlobals::golfData.roi_y, golfGlobals::golfTransmissionConsts::maximgwidth, golfGlobals::golfData.roi_height);
			
			Frame1 = cv::Mat(newRoih, golfGlobals::golfTransmissionConsts::maximgwidth, CV_8UC3, frameData1);
		}
		if(newRoiy2!=golfGlobals::golfData.roi_y2 || newRoih2!=golfGlobals::golfData.roi_height2)
		{
			golfGlobals::golfData.roi_y2 = newRoiy2;
			golfGlobals::golfData.roi_height2 = newRoih2;
			idsCam2->setROI(0, golfGlobals::golfData.roi_y2, golfGlobals::golfTransmissionConsts::maximgwidth, golfGlobals::golfData.roi_height2);
			Frame2 = cv::Mat(newRoih2, golfGlobals::golfTransmissionConsts::maximgwidth, CV_8UC3, frameData2);
		}
		if(newRoiy3!=golfGlobals::golfData.roi_y3 || newRoih3!=golfGlobals::golfData.roi_height3)
		{
			golfGlobals::golfData.roi_y3 = newRoiy3;
			golfGlobals::golfData.roi_height3 = newRoih3;
			idsCam3->setROI(0, golfGlobals::golfData.roi_y3, golfGlobals::golfTransmissionConsts::maximgwidth, golfGlobals::golfData.roi_height3);
			Frame3 = cv::Mat(newRoih3, golfGlobals::golfTransmissionConsts::maximgwidth, CV_8UC3, frameData3);
		}
		
		//golfGlobals::golfData.cam2Scale = (double)newRoih2/(double)newRoih;

		unsigned char r_low = ((unsigned char*)data)[offsetof(golfGlobals::golfTransmission,r_low)];
		unsigned char g_low = ((unsigned char*)data)[offsetof(golfGlobals::golfTransmission,g_low)];
		unsigned char b_low = ((unsigned char*)data)[offsetof(golfGlobals::golfTransmission,b_low)];
		unsigned char r_high = ((unsigned char*)data)[offsetof(golfGlobals::golfTransmission,r_high)];
		unsigned char g_high = ((unsigned char*)data)[offsetof(golfGlobals::golfTransmission,g_high)];
		unsigned char b_high = ((unsigned char*)data)[offsetof(golfGlobals::golfTransmission,b_high)];

		barMin = cv::Scalar(b_low,g_low,r_low);
		barMax = cv::Scalar(b_high,g_high,r_high);
		
		found_aoi1 = false;
		found_aoi2 = false;
		found_aoi3 = false;

		entrance.x = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,entrance_x));
		entrance.y = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,entrance_y));
		entrance.width = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,entrance_w));
		entrance.height = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,entrance_h));
		if(entrance.area()==0)
			entrance = cv::Rect(0,0,50,50);

		golfGlobals::golfData.ballsize = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,ballsize));
		
		golfGlobals::golfData.cam1Offsetx = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,cam1Offsetx));
		golfGlobals::golfData.cam1Offsety = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,cam1Offsety));

		golfGlobals::golfData.cam2Offsetx = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,cam2Offsetx));
		golfGlobals::golfData.cam2Offsety = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,cam2Offsety));
		golfGlobals::golfData.cam2Scale = *(double*)((char*)data+offsetof(golfGlobals::golfTransmission,cam2Scale));
		
		golfGlobals::golfData.cam3Offsetx = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,cam3Offsetx));
		golfGlobals::golfData.cam3Offsety = *(int*)((char*)data+offsetof(golfGlobals::golfTransmission,cam3Offsety));
		golfGlobals::golfData.cam3Scale = *(double*)((char*)data+offsetof(golfGlobals::golfTransmission,cam3Scale));
		
		golfGlobals::golfData.cam1thresh = *((char*)data+offsetof(golfGlobals::golfTransmission,cam1thresh));
		golfGlobals::golfData.cam2thresh = *((char*)data+offsetof(golfGlobals::golfTransmission,cam2thresh));
		golfGlobals::golfData.cam3thresh = *((char*)data+offsetof(golfGlobals::golfTransmission,cam3thresh));
		
		golfGlobals::golfData.ballmaskscale = *(double*)((char*)data+offsetof(golfGlobals::golfTransmission,ballmaskscale));

		classifyJob::cam1BallSize = golfGlobals::golfData.ballsize;
		classifyJob::cam2BallSize = int((double)classifyJob::cam1BallSize*golfGlobals::golfData.cam2Scale);
		classifyJob::cam3BallSize = int((double)classifyJob::cam1BallSize*golfGlobals::golfData.cam3Scale);

		golfGlobals::dataPipeInitialized = true;
	}

	if(!receivedBrands.empty() && !((char*)data)[offsetof(golfGlobals::golfTransmission,logoClk)])
	{
		((char*)data)[offsetof(golfGlobals::golfTransmission,logoClk)] = receivedBrands.back() + 1;
		receivedBrands.pop_back();
	}

	//Update Images
	if(!((char*)data)[offsetof(golfGlobals::golfTransmission,imagesClk)])
	{
		if(golfGlobals::golfData.render_img1 && frameData1)
		{
			downSample(Frame1, smallFrame1);
			if(golfGlobals::golfData.render_mask == 1)
			{
				cv::Mat mask, rgbmask;
				cv::inRange(smallFrame1, barMin, barMax, mask);
				cv::cvtColor(mask, smallFrame1, CV_GRAY2BGR);
			}
			else if(golfGlobals::golfData.render_mask == 2)
			{
				cv::Mat thresh(smallFrame1.rows, smallFrame1.cols, CV_8UC1);
				getGreen(smallFrame1, thresh);
				cv::threshold(thresh, thresh, golfGlobals::cam1thresh, 255, CV_THRESH_BINARY_INV);
				
				for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end(); ++ballit)
				{
					golfBall* ball = *ballit;
					cv::Rect ballROI = ball->getBallROI(4, 1);
					ballROI &= cv::Rect(cv::Point(), smallFrame1.size());
					if(ballROI.width != classifyJob::cam1BallSize/4 || ballROI.height != classifyJob::cam1BallSize/4)
						continue;
					cv::Mat Mask;
					cv::resize(classifyJob::cam1circleMask, Mask, cv::Size(ballROI.width, ballROI.height), 0, 0, CV_INTER_NN);
					cv::Mat ballimg;
					thresh(ballROI).copyTo(ballimg);
					cv::bitwise_and(ballimg, Mask, ballimg);
					ballimg.copyTo(thresh(ballROI));
				}
				cv::cvtColor(thresh, smallFrame1, CV_GRAY2BGR);
			}
			else
			{
				for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end(); ++ballit)
				{
					golfBall* ball = *ballit;

					cv::Rect ballROI = ball->getBallROI(4, 1);
					cv::rectangle(smallFrame1, ballROI, cv::Scalar(255,0,0), 1);
					cv::Rect smallTrack(ball->trackWindow.x/4, ball->trackWindow.y/4, ball->trackWindow.width/4, ball->trackWindow.height/4);
					cv::rectangle(smallFrame1, smallTrack, cv::Scalar(0,0,255), 1);
					
					if(golfGlobals::golfData.render_data && ball->identified)
					{
						int baseline;
						cv::Size textSize = cv::getTextSize(golfBall::brandNames[ball->brand], fontFace, fontScale, fontThickness, &baseline);
						baseline += fontThickness;
						// center the text
						cv::Point point3((int)(ballROI.x+ (ballROI.width-textSize.width)/2), (int) (ballROI.y+(ballROI.height-textSize.height)/2));
						cv::putText(smallFrame1, golfBall::brandNames[ball->brand], point3, fontFace, fontScale, cv::Scalar(100,0,200), fontThickness, CV_AA);
				
					}
				}
				cv::Rect smallEntrance(entrance.x/4, entrance.y/4, entrance.width/4, entrance.height/4);
				cv::rectangle(smallFrame1, smallEntrance, cv::Scalar(0,255,0), 1);
			}
			memcpy((char*)data+golfGlobals::golfTransmissionConsts::imgsOffset1, smallFrame1.data, smallFrame1.size().area()*3);
			((char*)data)[offsetof(golfGlobals::golfTransmission,imagesClk)] = 1;
			*(int*)((char*)data+offsetof(golfGlobals::golfTransmission,img_width1)) = smallFrame1.cols;
			*(int*)((char*)data+offsetof(golfGlobals::golfTransmission,img_height1)) = smallFrame1.rows;
		}
		if(golfGlobals::golfData.render_img2 && frameData2)
		{	
			downSample(Frame2, smallFrame2);
			if(golfGlobals::golfData.render_mask == 1)
			{
				cv::Mat mask, rgbmask;
				cv::inRange(smallFrame2, barMin, barMax, mask);
				cv::cvtColor(mask, smallFrame2, CV_GRAY2BGR);
			}
			else if(golfGlobals::golfData.render_mask == 2)
			{
				cv::Mat thresh(smallFrame2.rows, smallFrame2.cols, CV_8UC1);
				getGreen(smallFrame2, thresh);
				cv::threshold(thresh, thresh, golfGlobals::cam2thresh, 255, CV_THRESH_BINARY_INV);
				for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end(); ++ballit)
				{
					golfBall* ball = *ballit;
					cv::Rect ballROI = ball->getBallROI(4, 2);
					ballROI &= cv::Rect(cv::Point(), smallFrame2.size());
					if(ballROI.width != classifyJob::cam2BallSize/4 || ballROI.height != classifyJob::cam2BallSize/4)
						continue;
					cv::Mat Mask;
					cv::resize(classifyJob::cam2circleMask, Mask, cv::Size(ballROI.width, ballROI.height), 0, 0, CV_INTER_NN);
					cv::Mat ballimg;
					thresh(ballROI).copyTo(ballimg);
					cv::bitwise_and(ballimg, Mask, ballimg);
					ballimg.copyTo(thresh(ballROI));
				}
				cv::cvtColor(thresh, smallFrame2, CV_GRAY2BGR);
			}
			else
			{
				for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end(); ++ballit)
				{
					golfBall* ball = *ballit;
					cv::Rect roi = ball->getBallROI(4, 2);
					cv::rectangle(smallFrame2, roi, cv::Scalar(255,0,0), 1);
				}
			}
			memcpy((char*)data+golfGlobals::golfTransmissionConsts::imgsOffset2, smallFrame2.data, smallFrame2.size().area()*3);
			((char*)data)[offsetof(golfGlobals::golfTransmission,imagesClk)] = 1;
			*(int*)((char*)data+offsetof(golfGlobals::golfTransmission,img_width2)) = smallFrame2.cols;
			*(int*)((char*)data+offsetof(golfGlobals::golfTransmission,img_height2)) = smallFrame2.rows;
		}
		if(golfGlobals::golfData.render_img3 && frameData3)
		{	
			downSample(Frame3, smallFrame3);
			if(golfGlobals::golfData.render_mask == 1)
			{
				cv::Mat mask, rgbmask;
				cv::inRange(smallFrame3, barMin, barMax, mask);
				cv::cvtColor(mask, smallFrame3, CV_GRAY2BGR);
			}
			else if(golfGlobals::golfData.render_mask == 2)
			{
				cv::Mat thresh(smallFrame3.rows, smallFrame3.cols, CV_8UC1);
				getGreen(smallFrame3, thresh);
				cv::threshold(thresh, thresh, golfGlobals::cam3thresh, 255, CV_THRESH_BINARY_INV);
				for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end(); ++ballit)
				{
					golfBall* ball = *ballit;
					cv::Rect ballROI = ball->getBallROI(4, 3);
					ballROI &= cv::Rect(cv::Point(), smallFrame3.size());
					if(ballROI.width != classifyJob::cam3BallSize/4 || ballROI.height != classifyJob::cam3BallSize/4)
						continue;
					cv::Mat Mask;
					cv::resize(classifyJob::cam3circleMask, Mask, cv::Size(ballROI.width, ballROI.height), 0, 0, CV_INTER_NN);
					cv::Mat ballimg;
					thresh(ballROI).copyTo(ballimg);
					//cv::bitwise_and(ballimg, Mask, ballimg);
					ballimg.copyTo(thresh(ballROI));
				}
				cv::cvtColor(thresh, smallFrame3, CV_GRAY2BGR);
			}
			else
			{
				for(std::vector<golfBall*>::iterator ballit = trackedBalls.begin(); ballit!=trackedBalls.end(); ++ballit)
				{
					golfBall* ball = *ballit;
					cv::Rect roi = ball->getBallROI(4, 3);
					cv::rectangle(smallFrame3, roi, cv::Scalar(255,0,0), 1);
				}
			}
			memcpy((char*)data+golfGlobals::golfTransmissionConsts::imgsOffset3, smallFrame3.data, smallFrame3.size().area()*3);
			((char*)data)[offsetof(golfGlobals::golfTransmission,imagesClk)] = 1;
			*(int*)((char*)data+offsetof(golfGlobals::golfTransmission,img_width3)) = smallFrame3.cols;
			*(int*)((char*)data+offsetof(golfGlobals::golfTransmission,img_height3)) = smallFrame3.rows;
		}
		if(golfGlobals::golfData.render_img4 && frameData1)
		{
			memcpy((char*)data+golfGlobals::golfTransmissionConsts::imgsOffset4, Frame1.data, Frame1.size().area()*3);
			((char*)data)[offsetof(golfGlobals::golfTransmission,imagesClk)] = 1;
			*(int*)((char*)data+offsetof(golfGlobals::golfTransmission,img_width4)) = Frame1.cols;
			*(int*)((char*)data+offsetof(golfGlobals::golfTransmission,img_height4)) = Frame1.rows;
		}
	}

	UnmapViewOfFile(data);
	ReleaseMutex(golfGlobals::mutexPipe);
	return true;
}