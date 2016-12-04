#pragma once
#include "BrandDecisionRule.h"
#include <opencv2/opencv.hpp>

class golfBall;
class golfBallTracker;
class Gpu_Sift;
class IDS_Camera;
struct classifyJob;
struct classifyJobCC;
struct classifyJobNB;
struct classifyJobNike;

//#define DEBUGTIME
#if defined(DEBUGTIME)
#define QueryTimeStart(freq, start) \
	QueryPerformanceFrequency(&freq); \
	QueryPerformanceCounter(&start);

#define QueryTimeEnd(freq, start, end, perf, minperf, name) \
	QueryPerformanceCounter(&end); \
	perf = (double)((end.QuadPart - start.QuadPart) * 1000) / (double)freq.QuadPart; \
	if(perf>minperf) std::cout << "["name"]: " << perf << std::endl;
#endif

namespace golfGlobals
{
	extern cv::Mat templateImg;
	extern std::vector<std::vector<cv::Point> > templateContours;
	extern int max_queue_size;
	extern BrandDecisionRule DecisionRule;
	extern int cam1BallSize;
	extern int cam2BallSize;
	extern int cam3BallSize;
	extern int cam1thresh;
	extern int cam2thresh;
	extern int cam3thresh;

	extern bool dataPipe;
	extern void* filePipe;
	extern void* mutexPipe;
	extern bool dataPipeInitialized;

	extern golfBallTracker* tracker;
	extern Gpu_Sift* extractor;
	
	namespace golfTransmissionConsts
	{
		extern const int maximgwidth;
		extern const int maximgheight;
		extern const int imgsOffset1;
		extern const int imgsOffset2;
		extern const int imgsOffset3;
		extern const int imgsOffset4;
		extern const int fileSize;
		
		extern const int optionsOffset;
		extern const int optionsSize;

		extern const int controlsOffset;
		extern const int controlsSize;

		extern const int imgsizeoffset;
		extern const int imgsizesize;
	}

	struct golfTransmission
	{
		golfTransmission()
		{
			optionsClk = 0;
			controlsClk = 0;
			logoClk = 0;
			imagesClk = 0;
			render_img1 = 0;
			render_img2 = 0;
			render_img3 = 0;
			render_img4 = 0;
			render_data = 0;
			render_mask = 0;
			sort_brands = 0;
			sort_models = 0;
			img_width1 = golfTransmissionConsts::maximgwidth;
			img_height1 = golfTransmissionConsts::maximgheight;
			img_width2 = golfTransmissionConsts::maximgwidth;
			img_height2 = golfTransmissionConsts::maximgheight;
			img_width3 = golfTransmissionConsts::maximgwidth;
			img_height3 = golfTransmissionConsts::maximgheight;
			img_width4 = golfTransmissionConsts::maximgwidth;
			img_height4 = golfTransmissionConsts::maximgheight;
		}
		//Clk
		char optionsClk;
		char controlsClk;
		char logoClk;
		char imagesClk;

		//Controls
		char render_img1;
		char render_img2;
		char render_img3;
		char render_img4;
		char render_data;
		char render_mask;
		char sort_brands;
		char sort_models;

		//Img sizes
		int img_width1;
		int img_height1;
		int img_width2;
		int img_height2;
		int img_width3;
		int img_height3;
		int img_width4;
		int img_height4;

		//Options
		double sift_thresh;
		int max_compenents;
		int roi_y, roi_height, roi_y2, roi_height2, roi_y3, roi_height3;
		unsigned char r_low, g_low, b_low, r_high, g_high, b_high;
		int ballsize;
		int entrance_x, entrance_y, entrance_w, entrance_h;
		int num_frame_required_to_classify;
		int cam1Offsetx;
		int cam1Offsety;
		int cam2Offsetx;
		int cam2Offsety;
		int cam3Offsetx;
		int cam3Offsety;
		double cam2Scale;
		double cam3Scale;
		double ballmaskscale;
		unsigned char cam1thresh;
		unsigned char cam2thresh;
		unsigned char cam3thresh;
	};

	extern golfTransmission golfData;
}

extern void printLastError();
