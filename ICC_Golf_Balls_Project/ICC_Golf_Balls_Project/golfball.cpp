#include "golfball.h"
#include "tracker.h"
#include "classifier.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <fstream>

int firstBall = 0;

// static ball ID (ball counter)
int golfBall::idcounter = 0;
// vector of string with brand names
std::vector<std::string> golfBall::brandNames;
// Socket to connect with interface
SOCKET Plc = INVALID_SOCKET;
const int brandMap[14] = {0, 9, 14, 12, 2, 11, 3, 1, 6, 4, 7, 13, 15, 16}; //Map the brand numbers

/* 
0: Undetermined
9: Nike
14: Titleist
12: Bridgestone
2: Noodle
11:Callaway
3:Pinnacle
1:Precept
6:Srixon
4:Topflight
7:Taylormade
13:NXTTour
15:ProV1
16:ProV1X*/
//// Reads the files with brand names and stablish the PLC connection.\\

int golfBall::initialize()
{
	//Reading brands file
	std::ifstream brands("brands.txt");
	while(brands.good())
	{
		std::string line;
		std::getline(brands, line);
		brandNames.push_back(line);
	}
	if(brandNames.size()<3)
	{
		std::cout << "Couldn't load brands file or there aren't enough brands.\n Check brands.txt file!";
		return -1;
	}

	// Initialization of the PLC Communication\\

	//Initialize socket - WinSock lib.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult)
	{
		std::cout << "Error initializing winsock2.\n";
		return 0;
	}
	addrinfo *result = NULL, hints = {0};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo("192.168.1.85", "8081", &hints, &result);
	if(iResult)
	{
		std::cout << "Error calling getaddrinfo.\n";
		WSACleanup();
		return 0;
	}

	Plc = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(Plc == INVALID_SOCKET)
	{
		std::cout << "Error creating socket.\n";
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}

	bool noNagle = false;
	iResult = setsockopt(Plc, IPPROTO_TCP, TCP_NODELAY, (const char*)&noNagle, sizeof(noNagle));
	if(iResult)
	{
		std::cout << "Error setting the socket buffer mode.\n";
		return 0;
	}

	iResult = connect(Plc, result->ai_addr, (int)result->ai_addrlen);
	if(iResult == SOCKET_ERROR)
	{
		std::cout << "Couldn't connect to PLC.\n";
		closesocket(Plc);
		Plc = INVALID_SOCKET;
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}

	unsigned long noblock = 1;
	iResult = ioctlsocket(Plc, FIONBIO, &noblock);
	if(iResult != NO_ERROR)
	{
		std::cout << "Error setting the socket io mode to non-blocking.\n";
		return 0;
	}

	std::cout << "Successfully connected to the PLC.\n";

	return 0;
}

// wrapper for processing the job (processing threads)
static void* golfProcessThread(void* data)
{((ballProcess*)data)->processJobs(); return 0;}

// wrapper for release the ball
static void* cleanBall(void* b)
{delete (golfBall*)b; return 0;}

// Creates the thread (initialization)
ballProcess::ballProcess():job(NULL), nextjob(NULL), processing(true)
{
	pthread_cond_init(&condition, NULL);
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&thread, NULL, golfProcessThread, this);
	while(!queuejob.empty())
		queuejob.pop();
}

// Destructor
ballProcess::~ballProcess()
{
	processing = false;
	pthread_join(thread, NULL);
	pthread_cond_destroy(&condition);
	pthread_mutex_destroy(&mutex);
}

// Job execution
void ballProcess::processJobs()
{
	while(processing) // while thread active
	{
		pthread_mutex_lock(&mutex); // lock this thread
			if(queuejob.empty())
			{
				pthread_cond_wait(&condition, &mutex);
			}

			job = queuejob.front(); // assigning queued job as current job
			queuejob.pop();
			
		pthread_mutex_unlock(&mutex); // unlocking thread
		
		job->classify();
		delete job;
	}
}

// adding jobs to the queue
// note: Option to upgrade - > create a buffer of jobs
void ballProcess::addJob(classifyJob* newjob)
{
	// locking thread
	pthread_mutex_lock(&mutex);
	// overwritting the last next job
	queuejob.push(newjob);
	if(queuejob.size() > golfGlobals::max_queue_size)
	{
		delete queuejob.front();
		queuejob.pop();
	}

	pthread_cond_signal(&condition);
	pthread_mutex_unlock(&mutex);
}

// Initializes ball class
golfBall::golfBall(cv::Rect win):
	id(idcounter),
	identified(false),
	trackWindow(win),
	observed_times(0),
	numVotes(0),
	V1_test(false),
	brand(0),
	emptySlotsCount(0),
	cc_observed_times(0)
{
	templatevotes.assign(4, 0);
	cam1Votes.assign(brandNames.size(), 0);
	cam2Votes.assign(brandNames.size(), 0);
	cam3Votes.assign(brandNames.size(), 0);

	for(int i = 0; i<sizeof(processes)/sizeof(ballProcess*); ++i)
		((ballProcess**)processes)[i] = new ballProcess;
		
	idcounter = (idcounter+1)%255;
}

//Destructor of golfBall class
golfBall::~golfBall()
{
//	identified = true;
	for(int i = 0; i<sizeof(processes)/sizeof(ballProcess*); ++i)
		delete ((ballProcess**)processes)[i];
}

// Computes the Roi Rectangles for each camera
cv::Rect golfBall::getBallROI(int scale, int camera)
{
	cv::Rect ret;
	switch(camera)
	{
	case 1:
		return cv::Rect(
			(trackWindow.x-golfGlobals::golfData.ballsize+golfGlobals::golfData.cam1Offsetx)/scale,
			(trackWindow.y+golfGlobals::golfData.cam1Offsety)/scale,
			classifyJob::cam1BallSize/scale,
			classifyJob::cam1BallSize/scale);
	case 2:
		return cv::Rect(
			(int)((double)(trackWindow.x-golfGlobals::golfData.ballsize+golfGlobals::golfData.cam2Offsetx)*golfGlobals::golfData.cam2Scale)/scale,
			(int)((double)(trackWindow.y+golfGlobals::golfData.cam2Offsety)*golfGlobals::golfData.cam2Scale)/scale,
			classifyJob::cam2BallSize/scale,
			classifyJob::cam2BallSize/scale);
	case 3:
		return cv::Rect(
			(int)((double)(-trackWindow.x+golfGlobals::golfData.ballsize-golfGlobals::golfData.cam3Offsetx)*golfGlobals::golfData.cam3Scale)/scale,
			(int)((double)(trackWindow.y+golfGlobals::golfData.cam3Offsety)*golfGlobals::golfData.cam3Scale)/scale,
			classifyJob::cam3BallSize/scale,
			classifyJob::cam3BallSize/scale);
	}
	return cv::Rect();
}

// Verifies if the ball is completely visible wrt the tracker camera
bool golfBall::isAtImageEdge(int w)
{
	return trackWindow.x<w;//+golfGlobals::golfData.ballsize<w;
}

// Creates a detach thread that relaeases the ball.
// A thread is used since the application may need to wait until the classification is done.
void golfBall::ballClean()
{
	pthread_t thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //detached threads end after finishing running
	pthread_create(&thread, &attr, cleanBall, this);
	pthread_attr_destroy(&attr);
}

void golfBall::ballFinished2ndVer()
{
		golfGlobals::DecisionRule.ImportData(cam1Votes, cam2Votes, cam3Votes);
		brand = golfGlobals::DecisionRule.decision_rule();	
		golfGlobals::DecisionRule.clear();
}

// Leverages votes from all cameras after ball reaches the end of the visible track
void golfBall::ballFinished()
{
	// brand classification module \\

	if(emptySlotsCount >= 10) // don't send something for an empty slot
	{
		//std:: cout << "Emptyslot detected...\n";
		return;
	}
	if(golfGlobals::golfData.sort_brands)
	{
		if(golfBall::observed_times > 15)  
		{
			ballFinished2ndVer();
		}
		else
			brand = 0;
		if(templatevotes[3]>1)
			brand = 1;
		if(Plc == INVALID_SOCKET)
		{
			std::cout << "No connection to Plc!\n";
		}
		else
		{
			char brandbyte = brandMap[brand];
			if(brand == 0)
				brandbyte = 10;
			int iResult;
			do
			{
				iResult = send(Plc, &brandbyte, 1, 0);
				if(iResult == SOCKET_ERROR)
				{
					std::cout << "Lost connection to the PLC!\n";
					closesocket(Plc);
					Plc = INVALID_SOCKET;
					break;
				}
			}
			while(iResult < 1);
			//std::cout<< "Tell PLC: " << (int) brandbyte << std::endl;
		}
	}
	if(golfGlobals::golfData.sort_models)
	{
		//  computing maximum number of votes
		std::vector<int>::iterator i = std::max_element(templatevotes.begin(), templatevotes.end());
		if(*i>=classifyJob::Classification_Minvotes) // if there is a vote!
		{
			int index = (int)(i-templatevotes.begin()); //getting index (0 - NXT, 1 - V1, 2 -V1x)
			switch(index)
			{
			case 0: //0 - NXT
				if(templatevotes[2]>1)
					brand = 13;
				else
					brand = 11;
				break;
			case 1: //1 - V1
				if(templatevotes[2]<2)
					brand = 12;
				else
					brand = 13;
				break;
			case 2: //2 - V1x
				brand = 13;
				break;
			}
		}
		if(Plc == INVALID_SOCKET)
		{
			std::cout << "No connection to Plc!\n";
		}
		else
		{
			char brandbyte = brandMap[brand];
			if(brand == 0)
				brandbyte = 14;
			int iResult;
			do
			{
				iResult = send(Plc, &brandbyte, 1, 0);
				if(iResult == SOCKET_ERROR)
				{
					std::cout << "Lost connection to the PLC!\n";
					closesocket(Plc);
					Plc = INVALID_SOCKET;
					break;
				}
			}
			while(iResult < 1);
		}
	}
	
	ballClean(); //thread disposal of the ball
}

// add a new job for the model recognition module
void golfBall::addJobsCC(std::vector<classifyJobCC*>& jobs)
{
	for(std::vector<classifyJobCC*>::iterator i = jobs.begin(); i!=jobs.end(); ++i)
	{
		classifyJobCC* newjob = *i;
		if(!newjob->ball->identified)
		{
			newjob->ball->processes[newjob->camera][ballProcess::JOB_CC]->addJob(newjob);
		}
		else
			delete newjob;

		//Synchronous classify//
		////////////////////////
		//newjob->classify();
		//delete newjob;
		////////////////////////
	}
}

// add a new job for the Nike module
void golfBall::addJobsNike(std::vector<classifyJobNike*>& jobs)
{
	for(std::vector<classifyJobNike*>::iterator i = jobs.begin(); i!=jobs.end(); ++i)
	{
		classifyJobNike* newjob = *i;
		if(!newjob->ball->identified)
		{
			newjob->ball->processes[newjob->camera][ballProcess::JOB_CC]->addJob(newjob);
		}
		else
			delete newjob;

		//Synchronous classify//
		////////////////////////
		//newjob->classify();
		//delete newjob;
		////////////////////////
	}
}

// add a new job for the model recognition module
void golfBall::addJobsNB(std::vector<classifyJobNB*>& jobs)
{
	for(std::vector<classifyJobNB*>::iterator i = jobs.begin(); i!=jobs.end(); ++i)
	{
		classifyJobNB* newjob = *i;
		golfBall* b = golfGlobals::tracker->findBall(newjob->id); //checking if the ball is still visible

		if(b)
		{
			b->processes[newjob->camera][ballProcess::JOB_NB]->addJob(newjob);
		
			//Synchronous classify//
			////////////////////////
			//newjob->classify();
			//delete newjob;
			////////////////////////
		}
		else
		{
			delete newjob;
		}
	}
}