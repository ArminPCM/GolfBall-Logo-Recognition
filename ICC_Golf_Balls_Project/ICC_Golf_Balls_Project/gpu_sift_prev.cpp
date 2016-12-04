#include "Gpu_Sift.h"
#include "classifier.h"

#ifdef USE_SIFT
	#include "SiftGPU/SiftGPU.h"
	#include "SiftGPU/gl_core_3_0.h"

	void siftGPUKeypointsToOpenCVKeyPoints(std::vector<SiftGPU::SiftKeypoint>& siftkeys, std::vector<cv::KeyPoint>& cvkeys)
	{
		cvkeys.resize(siftkeys.size());
	
		std::vector<SiftGPU::SiftKeypoint>::iterator src = siftkeys.begin();
		std::vector<cv::KeyPoint>::iterator dst = cvkeys.begin();
		for(; src!=siftkeys.end(); ++src, ++dst)
		{
			dst->angle = src->o;
			dst->size = src->s;
			dst->pt.x = src->x;
			dst->pt.y = src->y;
			/*dst->class_id = 0;
			dst->octave = 0;
			dst->response = 0;*/
		}
	}
#endif

#ifdef USE_SIFT2
	#include "cudaImage.h"
	#include "cudaSift.h"

	void siftGPUKeypointsToOpenCVKeyPoints(SiftData& data, std::vector<cv::KeyPoint>& cvkeys, cv::Mat& descriptors)
	{
		descriptors = cv::Mat(data.numPts, 128, CV_32F);
		cvkeys.resize(data.numPts);
	
		SiftPoint* sift = data.h_data;
		std::vector<cv::KeyPoint>::iterator dst = cvkeys.begin();
		float* descdata = (float*)descriptors.data;
		for(; dst!=cvkeys.end(); ++sift, ++dst, descdata += 128)
		{
			dst->angle = sift->orientation;
			dst->size = sift->scale;
			dst->pt.x = sift->xpos;
			dst->pt.y = sift->ypos;
			/*dst->class_id = 0;
			dst->octave = 0;
			dst->response = 0;*/

			memcpy(descdata, sift->data, 128*sizeof(float));
		}
	}
#endif

#include <iostream>
#include <omp.h>


Gpu_Sift::Gpu_Sift()
{
	pthread_mutex_init(&client, NULL);
	submitData = false;
	ready = false;
	running = true;
}
Gpu_Sift::~Gpu_Sift()
{
	running = false;
	void* ret;
	pthread_join(thread, &ret);
	pthread_mutex_destroy(&client);
}

int Gpu_Sift::initialize()
{
	golfGlobals::golfData.sift_thresh = 5.5;
	pthread_create(&thread, NULL, ThreadFunction, this);

	std::vector<classifyJobNB*> jobs;
	cv::Mat empty = cv::Mat::zeros(100,100,CV_8UC1);
	classifyJobNB job(NULL, 0, empty, empty);
	jobs.push_back(&job);
	
	upload(jobs);
	while (!download(jobs));
	return 0;
}

static void releaseVector(std::vector<classifyJobNB*>& vec)
{
	for(std::vector<classifyJobNB*>::iterator i = vec.begin(); i!=vec.end(); ++i)
		delete *i;
	vec.clear();
}

bool Gpu_Sift::download(std::vector<classifyJobNB*>& njobs)
{
	pthread_mutex_lock(&client);
	
		if(!submitData)
		{
			pthread_mutex_unlock(&client); 
			return false;
		}
		submitData = false;
		njobs.swap(jobs_out);

	pthread_mutex_unlock(&client);
	return true;
}

void Gpu_Sift::upload(std::vector<classifyJobNB*>& njobs)
{
	pthread_mutex_lock(&client);

	releaseVector(jobs_in);
	std::swap(jobs_in, njobs);

	pthread_mutex_unlock(&client);
}

bool Gpu_Sift::IsReady()
{
	pthread_mutex_lock(&client);
	bool ret = ready;
	pthread_mutex_unlock(&client);
	return ret;
}

void Gpu_Sift::Thread()
{
	#ifdef USE_SIFT
		SiftGPU sift;
		char* argv[] = {"-fo", "-1", "-v", "0"/*, "-cuda", "0"*/};
		sift.ParseParam(/*6*/ 4, argv); // 6 args for cuda and 4 for non-cuda
		sift._dog_threshold = 0.04f;
		sift._edge_threshold = 5;
		sift.CreateContextGL();
		sift.VerifyContextGL();
	#endif
	
	#ifdef USE_SIFT2
		InitCuda();
	#endif
		
	#ifdef USE_SURF
		cv::gpu::SURF_GPU surf;
	#endif

	while(running)
	{
		pthread_mutex_lock(&client);

			if(jobs_in.empty())
			{
				pthread_mutex_unlock(&client);
				timespec time;
				time.tv_sec = 0;
				time.tv_nsec = 10000000;
				pthread_delay_np(&time);
				continue;
			}
			
			ready = false;
			jobs.swap(jobs_in);
		pthread_mutex_unlock(&client);

		#ifdef USE_SIFT
		{
			for(std::vector<classifyJobNB*>::iterator it = jobs.begin(); it!=jobs.end(); ++it)
			{
				classifyJobNB* job = *it;
				sift.RunSIFT(job->image->cols, job->image->rows, job->image->data, GL_LUMINANCE, GL_UNSIGNED_BYTE);
				int src_keypoint_count = sift.GetFeatureNum();
				if(src_keypoint_count>0)
				{
					std::vector<SiftGPU::SiftKeypoint> keypoints(src_keypoint_count);
					job->descriptors = cv::Mat(src_keypoint_count, 128, CV_32F);
					sift.GetFeatureVector(&keypoints[0], job->descriptors->data);
					siftGPUKeypointsToOpenCVKeyPoints(keypoints, job->keypoints);
				}
			}
		}
		#endif
		#ifdef USE_SIFT2
		{
			#pragma omp parallel for schedule(dynamic)
			for(int i = 0; i<jobs.size(); ++i)
			{
				classifyJobNB* job = jobs[i];
				SetDevice(0);
				/*int pixels = cv::countNonZero(job->imgThresh);
				if(pixels < job->image.size().area()/200)
					continue;*/

				cv::gpu::GpuMat image;
				image.upload(job->image);
				image.convertTo(image, CV_32FC1);

				CudaImage img;
				img.Allocate(image.cols, image.rows, (int)(image.step/image.elemSize()), false, (float*)image.data, NULL);
				
				SiftData siftdata;
				InitSiftData(siftdata, 2048, true, true);
				ExtractSift(siftdata, img, 5, 0.0, (float)golfGlobals::golfData.sift_thresh, 0.01f);
				
				siftGPUKeypointsToOpenCVKeyPoints(siftdata, job->keypoints, job->descriptors);
				FreeSiftData(siftdata);
			}
		}
		#endif

		#ifdef USE_SURF
		{
			for(std::vector<classifyJobNB*>::iterator it = jobs.begin(); it!=jobs.end(); ++it)
			{
				classifyJobNB* job = *it;
				cv::gpu::GpuMat image, keypoints, descriptors;

				image.upload(job->image);
				surf(image, cv::gpu::GpuMat(), keypoints, descriptors, false);
				surf.downloadKeypoints(keypoints, job->keypoints);
				descriptors.download(job->descriptors);
			}
		}
		#endif

		pthread_mutex_lock(&client);

			//#define ShowKeypoints
			#ifdef ShowKeypoints
				for(std::vector<classifyJobNB*>::iterator i = jobs.begin(); i!=jobs.end(); ++i)
				{
					classifyJobNB* j = *i;
					std::stringstream ss;
					ss << "KeyPoints " << j->camera;
					timespec time;
					time.tv_sec = 0;
					time.tv_nsec = 1000000;
					pthread_delay_np(&time);
					cv::Mat showkeypoints;
					cv::drawKeypoints(j->image, j->keypoints, showkeypoints);
					cv::imshow(ss.str(), showkeypoints);
					cv::waitKey(100);
				}
			#endif

			if(submitData)
				releaseVector(jobs_out);
			ready = true;
			submitData = true;
			jobs_out.swap(jobs);

		pthread_mutex_unlock(&client);
	}
}

void* Gpu_Sift::ThreadFunction(void* arg)
{
	((Gpu_Sift*)arg)->Thread();
	return 0;
}