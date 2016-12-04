#pragma once

#include "globals.h"

#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <queue>

struct ballProcess
{
	enum{JOB_CC = 0, JOB_NB = 1};
	ballProcess(); 
	~ballProcess();
	void processJobs(); // executes the job (in a its thread)
	void addJob(classifyJob* job);
	pthread_t thread; // thread
	pthread_mutex_t mutex; // lock
	pthread_cond_t condition;
	classifyJob* job, *nextjob; // current and queued job
	std::queue<classifyJob*> queuejob; // queue for storing more jobs
	bool processing; // mantains thread active
};

class golfBall
{
	typedef std::vector<std::vector<cv::KeyPoint>> KeypointArray;
    golfBall (const golfBall& other);
    golfBall& operator= (golfBall other);
	static int idcounter;

	//One processor for each camera*each classifier type
	ballProcess* processes[3][2];
public:
	int emptySlotsCount;
	static std::vector<std::string> brandNames;

	int id;
	bool identified;
	bool V1_test;
	cv::Rect trackWindow;

	int observed_times, numVotes, brand, cc_observed_times;
	std::vector<int> cam1Votes, cam2Votes, cam3Votes;
	std::vector<int> templatevotes;
	
	golfBall(cv::Rect win);
	~golfBall();
	// Computes the Roi Rectangles for each camera
	cv::Rect getBallROI(int scale, int camera);
	// Verifies if the ball is completely visible wrt the tracker camera
	bool isAtImageEdge(int w);
	
	void ballClean();
	void ballFinished();
	void ballFinished2ndVer();
	static void addJobsCC(std::vector<classifyJobCC*>& jobs);
	static void addJobsNike(std::vector<classifyJobNike*>& jobs);
	static void addJobsNB(std::vector<classifyJobNB*>& jobs);
	// Reads the files with brand names and stablish the PLC connection.
	static int initialize();
};