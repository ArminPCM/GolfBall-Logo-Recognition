#include "IDS_Camera.h"
#include "globals.h"

#ifndef NO_UEYE
	#include <uEye.h>
	#pragma comment(lib, "uEye_api_64.lib")
	#pragma comment(lib, "ueye_tools_64.lib")
#endif

#include <iostream>
#include <sstream>

IDS_Camera::IDS_Camera():
	m_hCam(0),
#ifndef NO_UEYE
	m_nRenderMode(IS_RENDER_FIT_TO_WINDOW),
	m_nColorMode(IS_CM_BGR8_PACKED),
#endif
	m_nBitsPerPixel(24),
	nMemID(0),
	m_filename(NULL),
	m_filecount(0),
	m_frameindex(311),
	m_pcImageMemory(NULL)
{
#ifndef NO_UEYE
	newFrame = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
}


IDS_Camera::~IDS_Camera(void)
{
#ifndef NO_UEYE
	is_ExitCamera (m_hCam);
#endif
}

int IDS_Camera::setROI(int x, int y, int width, int height)
{
	if(m_filename || !m_hCam) return -1;
#ifndef NO_UEYE
	IS_RECT rectAOI;
	rectAOI.s32X     = x;
	rectAOI.s32Y     = y;
	rectAOI.s32Width = width;
	rectAOI.s32Height = height;
	m_nSizeX = width;
	m_nSizeY = height;

	/*while(*/is_AOI( m_hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));//);

#endif
	return 0;
}

int IDS_Camera::resetROI()
{
	if(m_filename || !m_hCam) return -1;
#ifndef NO_UEYE
	IS_RECT rectAOI;
	rectAOI.s32X     = 0;
	rectAOI.s32Y     = 0;
	rectAOI.s32Width = m_nMaxSizeX;
	rectAOI.s32Height = m_nMaxSizeY;
	m_nSizeX = m_nMaxSizeX;
	m_nSizeY = m_nMaxSizeY;

	int m_Ret = is_AOI( m_hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));
	if(m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't reset Region Of Interest: " << m_Ret << std::endl;
		return -1;
	}
#endif
	return 0;
}

void IDS_Camera::setinifile(char* file)
{
	if(m_filename || !m_hCam) return;
#ifndef NO_UEYE
	std::wstringstream s;
	s << file;
	
	int m_Ret = is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, (void*)s.str().c_str(), NULL);
	if(m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't set camera parameters.\n";
	}
#endif
}

bool IDS_Camera::initiCamera(HIDS camid, char SerNo[16])
{
	// At least one camera must be available
	INT nNumCam;
	// Create new list with suitable size
	UEYE_CAMERA_LIST* pucl;
	if( is_GetNumberOfCameras( &nNumCam ) == IS_SUCCESS) 
	{
		if( nNumCam == 3 ) {
			pucl = (UEYE_CAMERA_LIST*) new BYTE [sizeof (DWORD) + nNumCam * sizeof (UEYE_CAMERA_INFO)];
			pucl->dwCount = nNumCam;

			//Retrieve camera info

			if (is_GetCameraList(pucl) == IS_SUCCESS) {
				int iCamera;
				for (iCamera = 0; iCamera < (int)pucl->dwCount; iCamera++) {
					//Test output of camera info on the screen
					//printf("Camera %i Serial: %s, Str: %s\n", iCamera, pucl->uci[iCamera].SerNo, SerNo);
					if(strcmp(pucl->uci[iCamera].SerNo, SerNo)==0)
					{

						camid = pucl->uci[iCamera].dwDeviceID;
						std::cout << "Found the device...\n";
						break;
					}
				}
			}
			
			delete [] pucl;
		}
	}
	if(m_filename) return true;
#ifndef NO_UEYE
	int m_Ret;
	if(m_hCam!=0)
		is_ExitCamera(m_hCam);

	m_hCam = camid | IS_USE_DEVICE_ID;
	m_Ret = is_InitCamera (&m_hCam, NULL);    // init room
	if (m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't initilize the camera!\n";
		return false;
	}

	// retrieve original image size
	SENSORINFO sInfo;
	is_GetSensorInfo (m_hCam, &sInfo);
	m_nMaxSizeX = sInfo.nMaxWidth;
	m_nMaxSizeY = sInfo.nMaxHeight;
	m_nSizeX = sInfo.nMaxWidth;
	m_nSizeY = sInfo.nMaxHeight;

	// Memory Initialization
	/*for (int i=0; i<maxBufSize; i++)
	{
		m_Ret = is_AllocImageMem (m_hCam, m_nMaxSizeX, m_nMaxSizeY, m_nBitsPerPixel, &m_pcImageMemory[i], &m_lMemoryId[i]);
		if(m_Ret != IS_SUCCESS)
		{
		std::cout << "Couldn't allocate memory for buffer.\n";
		return false;
		}

		m_Ret = is_AddToSequence (m_hCam, m_pcImageMemory[i], m_lMemoryId[i]);
		if(m_Ret != IS_SUCCESS)
		{
			std::cout << "Couldn't add buffer to sequence.\n";
			return false;
		}
	}*/

	m_Ret = is_AllocImageMem (m_hCam, m_nMaxSizeX, m_nMaxSizeY, m_nBitsPerPixel, &m_pcImageMemory, &m_lMemoryId);
	if(m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't allocate memory for buffer.\n";
		return false;
	}

	m_Ret = is_SetImageMem (m_hCam, m_pcImageMemory, m_lMemoryId);
	if(m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't allocate memory for buffer.\n";
		return false;
	}
	
    m_Ret = is_SetTimeout( m_hCam, IS_TRIGGER_TIMEOUT, 500);
	if(m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't set timeout.\n";
		return false;
	}

	m_Ret = is_SetExternalTrigger(m_hCam, IS_SET_TRIGGER_SOFTWARE);
	if(m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't turn external trigger off.\n";
		return false;
	}

	m_Ret = is_InitEvent(m_hCam, newFrame, IS_SET_EVENT_FRAME);
	if(m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't init the new frame event.\n";
		return false;
	}

	m_Ret = is_EnableEvent(m_hCam, IS_SET_EVENT_FRAME);
	if(m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't enable new frame event.\n";
		return false;
	}
#endif
	return true;
}

void IDS_Camera::startCapture()
{
	if(m_filename || !m_hCam) return;
#ifndef NO_UEYE
	int m_Ret = is_CaptureVideo(m_hCam, IS_WAIT);
	if(m_Ret != IS_SUCCESS)
	{
		std::cout << "Couldn't set free run.\n";
	}
#endif
}

void IDS_Camera::setCaptureFiles(const char* name, int number)
{
	m_filename = name;
	m_filecount = number;
}

char* IDS_Camera::freezeFrame()
{
	/*if(m_filename)
	{
		Sleep(10);
		std::stringstream ss;
		ss << m_filename << "\\" << m_frameindex << ".bmp";
		//std::cout << m_filename << "\\" << m_frameindex << ".bmp" << std::endl;
		cv::Mat im = cv::imread(ss.str());
		size_t bytes = im.step*im.rows;
		char* data = new char[bytes];
		memcpy(data, im.data, bytes);

		m_frameindex = (m_frameindex + 1) % m_filecount;
		return data;
	}*/
#ifndef NO_UEYE
	/*else */if(m_hCam != 0)
	{
		if(WaitForSingleObject(newFrame, 10) == WAIT_OBJECT_0)
		{
			int m_Ret = is_LockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, m_pcImageMemory);
			if(m_Ret)
				std::cout << "Lock error " << m_Ret << std::endl;

			return m_pcImageMemory;
		}
		else
			return NULL;
	}
	return NULL;
#endif
}

void IDS_Camera::unlockImage(char* data)
{
#ifndef NO_UEYE
	if(m_filename)
	{
		if(data)
			delete[] data;
	}
	else if(m_hCam && data) 
	{
		int m_Ret = is_UnlockSeqBuf (m_hCam, IS_IGNORE_PARAMETER, data);
		if(m_Ret)
			std::cout << "Unlock error " << m_Ret << std::endl;
	}
#endif
}

void IDS_Camera::Cleanup()
{
	if(m_hCam != 0)
	{
		is_ExitCamera(m_hCam);
	}
}