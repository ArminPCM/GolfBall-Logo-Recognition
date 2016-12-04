#pragma once
#include "globals.h"

#include <opencv2/core/core.hpp>

class golfBallTracker
{
public:
	IDS_Camera *idsCam1, *idsCam2, *idsCam3;
	cv::Mat Frame1, smallFrame1;
	cv::Mat Frame2, smallFrame2;
	cv::Mat Frame3, smallFrame3;
	char *frameData1, *frameData2, *frameData3;
	bool found_aoi1, found_aoi2, found_aoi3;

	std::vector<golfBall*> trackedBalls;
	std::vector<char> receivedBrands;
	
	cv::Rect entrance;
	int fontFace;
	double fontScale;
	int fontThickness;
	cv::Scalar barMin, barMax;

	golfBallTracker();
	int initialize();
	void trackBalls();
	void searchForBars();
	golfBall* findBall(int id);
	void update();
	void releaseFrame();
	bool updateTransmission();
	void downSample(cv::Mat& in, cv::Mat& out);
	bool findAOI(cv::Mat& frame, IDS_Camera*);
	//void getBallImages(std::vector<classifyJobCC*>& balls, std::vector<classifyJobNB*>& sift_balls);
	void getBallImages(std::vector<classifyJobCC*>& balls);
	void getBallImages(std::vector<classifyJobNB*>& sift_balls, std::vector<classifyJobNike*>& balls);
	void stopCameras();
};