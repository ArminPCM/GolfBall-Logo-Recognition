#pragma once

//#define NO_UEYE
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef unsigned long HIDS;

class IDS_Camera
{
public:
// Public Data Members
	HIDS m_hCam;				//IDS camera handle
	void* newFrame;
	int m_nRenderMode;					// render  mode
	int m_nColorMode;					// Y8/RGB16/RGB24/REG32
	int m_nBitsPerPixel;				// number of bits needed store one pixel
	int m_nMaxSizeX;						// width of video
	int m_nMaxSizeY;						// height of video
	int m_nSizeX;						// width of video
	int m_nSizeY;						// height of video
	int m_lMemoryId;        // grabber memory - buffer   ID
	char* m_pcImageMemory;  // grabber memory - pointer to buffer
	int nMemID;							// memory ID

	const char* m_filename;
	int m_filecount;
	int m_frameindex;
	
	// Constructor & Destructor
	IDS_Camera(void);
	~IDS_Camera(void);

	void setCaptureFiles(const char* name, int number);
	int setROI(int x, int y, int width, int height);
	int resetROI();
	bool initiCamera(HIDS, char SerNo[16]);
	bool lockImage(cv::Mat&);
	void unlockImage(cv::Mat&);
	void unlockImage(char*);
	void setinifile(char* file);
	char* freezeFrame();
	void startCapture();
	void Cleanup();
};
