#pragma once
#include "globals.h"
#include <pthread.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/gpu/gpu.hpp>

#define USE_SIFT2

class Gpu_Sift
{
public:
	typedef std::vector<std::vector<cv::KeyPoint>> KeypointArray;

private:
	static void* ThreadFunction(void*);
	//Messing with this stuff is not threadsafe.
	pthread_t thread;
	pthread_mutex_t client;
	bool ready, submitData, running;

	std::vector<classifyJobNB*> jobs_in, jobs, jobs_out;

public:
	//External members
	Gpu_Sift();
	~Gpu_Sift();
	int initialize();
	void upload(std::vector<classifyJobNB*>& jobs);
	bool download(std::vector<classifyJobNB*>& jobs);
	void Thread();
	void UseSurf();
	void UseSift();
	bool IsReady();
private:
	//Class moving/copying unimplemented
    Gpu_Sift (const Gpu_Sift& other);
    Gpu_Sift& operator= (Gpu_Sift other);
};
