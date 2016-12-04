#ifndef Classifer_H
#define Classifer_H
#pragma once
#include "globals.h"

#include <opencv2/opencv.hpp>
#include <pthread.h>

struct CrossCorrelationJob
{
	CrossCorrelationJob(cv::Mat& itemplate, cv::Mat& icropped):
		templateImage(itemplate),
		croppedImage(icropped),
		minVal(0),
		maxVal(0)
	{}

	cv::Mat &templateImage, &croppedImage;
	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	int Model, Angle, ConnectedComponent;
	pthread_t thread;
	void work();
};

struct classifyJob
{
	struct component
	{
		cv::Mat croppedImage;
		int angle;
		cv::Point offset;
	};
	classifyJob(golfBall* b, int cam, cv::Mat& img, cv::Mat& imgThre);
	int steps, id, camera;
	golfBall* ball;
	cv::Mat image, imgThresh;
	std::vector<component> components;

	virtual void classify() = 0;
	virtual ~classifyJob() {};

	static void findComponents(int camera, cv::Mat& green, cv::Mat& imgThresh, std::vector<classifyJob::component>&);
	static int classifyJob::thresholdImage(int camera, cv::Mat& image, cv::Mat& imgThresh, int& isEmptySlot, golfBall* ball);

	static int initialize();
	static bool compressPCA(const cv::Mat& testset, cv::Mat& compressed);
	//static cv::PCA createCompressPCA(cv::Mat pcaset, int maxComponents, const cv::Mat& testset, cv::Mat& compressed);
	static float template_w;
	static float template_h;
	static cv::Mat dilationKernel;
	static cv::Mat cam1circleMask, cam2circleMask, cam3circleMask;
	static int cam1BallSize, cam2BallSize, cam3BallSize, cam3BallSize_tmp;
	static std::vector<std::vector<cv::Mat>> Templates;
	static cv::PCA pca;
	static cv::NormalBayesClassifier nbc;

	static int Classification_MaxvotesNXT;
	static int Classification_Minvotes;
	static int Classification_MaxvotesV1;
	static int Classification_Maxvotes;

	static double Classification_Threshold;
	static double Classification_ThresholdNike;
	static double Classification_ratioThreshold;
	static double Classification_Threshold_Superb;
	static double Classification_Threshold_NoRead;
};

struct classifyJobCC : public classifyJob
{
	classifyJobCC(golfBall* b, int cam, cv::Mat& img, cv::Mat& imgThre): classifyJob(b, cam, img, imgThre){}
	~classifyJobCC(){};
	void classify();
};

struct classifyJobNike : public classifyJob
{
	classifyJobNike(golfBall* b, int cam, cv::Mat& img, cv::Mat& imgThre): classifyJob(b, cam, img, imgThre){}
	~classifyJobNike(){};
	void classify();
};

struct classifyJobNB : public classifyJob
{
	classifyJobNB(golfBall* b, int cam, cv::Mat& img, cv::Mat& imgThre): classifyJob(b, cam, img, imgThre){}
	~classifyJobNB(){};
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;
	void classify();
};
#endif //#ifndef Classifer_H