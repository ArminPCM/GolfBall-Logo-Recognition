// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include <cstring>
#include <cstddef>

namespace ICC_interface
{
	extern const wchar_t* settingFile;

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
		void setDefaults()
		{
			sift_thresh = 5.5;
			max_compenents = 90;
			roi_y = 0;
			roi_height = golfTransmissionConsts::maximgheight;
			roi_y2 = 0;
			roi_height2 = golfTransmissionConsts::maximgheight;
			roi_y3 = 0;
			roi_height3 = golfTransmissionConsts::maximgheight;
			r_low = g_low = b_low = 0;
			r_high = g_high = b_high = 255;
			ballsize = 100;
			entrance_x = entrance_y = 0;
			entrance_w = entrance_h = 50;
			num_frame_required_to_classify = 8;
			cam1Offsetx = 0;
			cam1Offsety = 0;
			cam2Offsetx = 0;
			cam2Offsety = 0;
			cam3Offsetx = 0;
			cam3Offsety = 0;
			cam2Scale = 1.0;
			cam3Scale = 1.0;
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

	ref class ControlWindow;
	ref class StatisticsWindow;
	ref class OptionsWindow;
	ref class SnapshotWindow;
}