#include "classifier.h"
#include "golfball.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max
#include <iostream>
#include <math.h>
#include <fstream>

float classifyJob::template_w;
float classifyJob::template_h;

std::vector<std::vector<cv::Mat>> classifyJob::Templates;

// PCA
cv::PCA classifyJob::pca;
cv::NormalBayesClassifier classifyJob::nbc;


static const int keypoints_threshold = 10;
// Global variables
int angleCorrection, rowsMin, rowsMax, mean_thresh, tolCC_error, dil_iter;
cv::Mat classifyJob::dilationKernel;
cv::Mat classifyJob::cam1circleMask, classifyJob::cam2circleMask, classifyJob::cam3circleMask;
int classifyJob::cam1BallSize, classifyJob::cam2BallSize, classifyJob::cam3BallSize;
int cam3BallSize;
/*
double classifyJob::Classification_Threshold = 0.6;
double classifyJob::Classification_ratioThreshold = 1.20;
int classifyJob::Classification_Maxvotes = 15;
int classifyJob::cam3BallSize_tmp;
*/


double classifyJob::Classification_Threshold_Superb = 0.75;
double classifyJob::Classification_Threshold = 0.65;
double classifyJob::Classification_ThresholdNike = 0.59;
double classifyJob::Classification_ratioThreshold = 1.05;
int classifyJob::Classification_MaxvotesNXT = 10;
int classifyJob::Classification_Minvotes = 2;
int classifyJob::Classification_MaxvotesV1 = 10;
int classifyJob::Classification_Maxvotes = 3;

int golfGlobals::cam1BallSize, golfGlobals::cam2BallSize, golfGlobals::cam3BallSize;
int golfGlobals::cam1thresh, golfGlobals::cam2thresh, golfGlobals::cam3thresh;


void loadParameters()
{
	std::ifstream paramsFile;
	if(golfGlobals::golfData.sort_brands)
	{
		paramsFile.open ("paramsFileNike.txt");
		classifyJob::dilationKernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3), cv::Point(-1,-1));
	}
	else
	{
		paramsFile.open ("paramsFileModels.txt");
	}
	float cam1Ballscale, cam2Ballscale, cam3Ballscale; /*new*/
	paramsFile >> angleCorrection;
	paramsFile >> rowsMin;
	paramsFile >> rowsMax;
	paramsFile >> tolCC_error;
	paramsFile >> mean_thresh;
	paramsFile >> dil_iter;
	paramsFile >> cam1Ballscale;/*new*/
	paramsFile >> cam2Ballscale;/*new*/
	paramsFile >> cam3Ballscale;/*new*/

	paramsFile >> golfGlobals::cam1thresh;
	paramsFile >> golfGlobals::cam2thresh;
	paramsFile >> golfGlobals::cam3thresh;

	paramsFile.close();

	/*new*/
	golfGlobals::cam1BallSize = (int)classifyJob::cam1BallSize*cam1Ballscale; //we are using golfGlobals::cam1BallSize as scale factor
	golfGlobals::cam2BallSize = (int)classifyJob::cam2BallSize*cam2Ballscale; //we are using golfGlobals::cam1BallSize as scale factor
	golfGlobals::cam3BallSize = (int)classifyJob::cam3BallSize*cam3Ballscale; //we are using golfGlobals::cam1BallSize as scale factor

	golfGlobals::cam1BallSize+=(golfGlobals::cam1BallSize%2);
	golfGlobals::cam2BallSize+=(golfGlobals::cam2BallSize%2);
	golfGlobals::cam3BallSize+=(golfGlobals::cam3BallSize%2);

	//classifyJob::cam1circleMask = cv::Mat::zeros(golfGlobals::cam1BallSize, golfGlobals::cam1BallSize, CV_8UC1);
	classifyJob::cam1circleMask = cv::Mat::zeros( (int)classifyJob::cam1BallSize, (int)classifyJob::cam1BallSize, CV_8UC1);
	classifyJob::cam2circleMask = cv::Mat::zeros(golfGlobals::cam2BallSize, golfGlobals::cam2BallSize, CV_8UC1);
	classifyJob::cam3circleMask = cv::Mat::zeros(golfGlobals::cam3BallSize, golfGlobals::cam3BallSize, CV_8UC1);
	golfGlobals::golfData.ballmaskscale = 0.5;

	cv::circle(classifyJob::cam1circleMask, cv::Point( (int)(classifyJob::cam1BallSize/2),  (int) (classifyJob::cam1BallSize/2)),  (int) ((double)classifyJob::cam1BallSize*golfGlobals::golfData.ballmaskscale+20), cv::Scalar(255), -1);
	cv::circle(classifyJob::cam2circleMask, cv::Point(golfGlobals::cam2BallSize/2, golfGlobals::cam2BallSize/2), int((double)golfGlobals::cam2BallSize*golfGlobals::golfData.ballmaskscale+20), cv::Scalar(255), -1);
	cv::circle(classifyJob::cam3circleMask, cv::Point(golfGlobals::cam3BallSize/2, golfGlobals::cam3BallSize/2), int((double)golfGlobals::cam3BallSize*golfGlobals::golfData.ballmaskscale+20), cv::Scalar(255), -1);
}

int classifyJob::initialize()
{	
	// Classes and Descs
	
	std::cout << "PCA Started...\n";
	cv::FileStorage file2("pca6.yml", cv::FileStorage::READ);
	if(!file2.isOpened())
	{
		std::cout << "Couldn't load the PCA file.\n";
		return -1;
	}

	file2["eigenvalues"] >> pca.eigenvalues;
	file2["eigenvectors"] >> pca.eigenvectors;
	file2["mean"] >> pca.mean;

	if(pca.eigenvalues.empty())
	{
		std::cout << "Couldn't load the eigenvalues\n";
		return -1;
	}
	if(pca.eigenvectors.empty())
	{
		std::cout << "Couldn't load the eigenvectors\n";
		return -1;
	}
	if(pca.mean.empty())
	{
		std::cout << "Couldn't load the means\n";
		return -1;
	}

	nbc.load("nbcBrands6.cls");

	//Read templates
	std::vector<std::string> templateNames;
	std::ifstream templates("templates.txt");
	while(templates.good())
	{
		std::string line;
		std::getline(templates, line);
		templateNames.push_back(line);
	}
	
	Templates.resize(templateNames.size());

	for(int i=0; i<templateNames.size(); ++i){
		Templates[i].reserve(360);
		for(int j=0; j<360; ++j){		
			std::stringstream name;
			name << "Templates/" << templateNames[i] << j+1 << ".png";
			cv::Mat image = cv::imread(name.str(), CV_LOAD_IMAGE_UNCHANGED);
			Templates[i].push_back(image);
		}	
	}

	int w1 = Templates[4][0].cols, w2 = Templates[5][0].cols, h1 = Templates[4][0].rows, h2 = Templates[5][0].rows;
	classifyJob::template_w = (float)((w1>w2)? w1:w2);
	classifyJob::template_h = (float)((h1>h2)? h1:h2);
	
	//classifyJob::template_w += 5;
	//classifyJob::template_h += 5;

	loadParameters();

	golfGlobals::templateImg = Templates[6][1].clone();
	cv::findContours( golfGlobals::templateImg, golfGlobals::templateContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	return 0;
}

void CrossCorrelationJob::work()
{
	int result_cols, result_rows, offset_x = 0, offset_y = 0;
	if (croppedImage.cols < templateImage.cols){
		offset_y = (int) ( ceil((templateImage.cols - croppedImage.cols)/2.0) );
		result_cols =  2*offset_y+croppedImage.cols - templateImage.cols + 1;
	}
	else
		result_cols =  croppedImage.cols - templateImage.cols + 1;

	if(croppedImage.rows < templateImage.rows){
		offset_x = (int) ( ceil((templateImage.rows - croppedImage.rows)/2.0) );
		result_rows = 2*offset_x + croppedImage.rows - templateImage.rows + 1;
	}
	else
		result_rows =  croppedImage.rows - templateImage.rows + 1;	

	cv::Mat result;
	result.create( result_cols, result_rows, CV_32FC1 );	
	if (offset_x > 0 || offset_y > 0){	
		cv::Mat testFrame;
		testFrame = cv::Mat::zeros(2*offset_x + croppedImage.rows, 2*offset_y + croppedImage.cols, croppedImage.type());
		croppedImage.copyTo(testFrame(cv::Rect(cv::Point(offset_y, offset_x), croppedImage.size())));
		cv::matchTemplate(testFrame, templateImage, result, CV_TM_CCOEFF_NORMED);
	}
	else
		cv::matchTemplate(croppedImage, templateImage, result, CV_TM_CCOEFF_NORMED);

	cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
}

static void* CrossCorrelationThread(void* data)
{((CrossCorrelationJob*)data)->work(); return 0;}

static int round(double x)
{
	double r = fmod(x, 1);
	if(r<0)
	{
		if(r<-0.5)
			return (int)floor(x);
		else
			return (int)x;
	}
	else
	{
		if(r>0.5)
			return (int)ceil(x);
		else
			return (int)x;
	}
}

cv::Rect cam1emptyROI, boundingBox, cam2emptyROI, cam3emptyROI;
/*new (from B) */
int classifyJob::thresholdImage(int camera, cv::Mat& image, cv::Mat& imgThresh, int& isEmptySlot, golfBall* ball)
{
	int pixels = 0;
	cv::Mat dilatedImg;
	std::vector<std::vector<cv::Point> > contours;
	switch(camera)
	{			
	case 1:
		isEmptySlot = 1;
		cv::threshold(image, imgThresh, golfGlobals::cam1thresh, 255, CV_THRESH_BINARY_INV);
		cv::bitwise_and(imgThresh, cam1circleMask, imgThresh);		
		pixels = cv::countNonZero(imgThresh);
		// Mostafa's no ball detection code
		cam1emptyROI.x = 0; cam1emptyROI.width = imgThresh.cols; cam1emptyROI.y = imgThresh.rows/4; cam1emptyROI.height = 2*imgThresh.rows/3;
		cv::dilate(imgThresh(cam1emptyROI), dilatedImg, dilationKernel, cv::Point(-1,-1), 4);
		cv::findContours( dilatedImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		for(int i=0; i<contours.size(); i++)
		{
			boundingBox = boundingRect(contours[i]);
			if(boundingBox.height/(boundingBox.width) < 0.09 && boundingBox.width > 0.8*imgThresh.cols)
			{
				if(cv::countNonZero(dilatedImg(boundingBox)) > 0.90 * boundingBox.area())
				{
					isEmptySlot = -1;
					ball->emptySlotsCount++;
					return 0;
				}
			}
		}
		cv::resize(imgThresh, imgThresh, cv::Size(golfGlobals::cam1BallSize, golfGlobals::cam1BallSize), 0, 0, CV_INTER_LINEAR);
		cv::threshold(imgThresh, imgThresh, golfGlobals::cam2thresh, 255, CV_THRESH_BINARY);
		break;
	case 2:
		if(isEmptySlot == 1)
		{
			cv::threshold(image, imgThresh, golfGlobals::cam2thresh, 255, CV_THRESH_BINARY_INV);
			cv::resize(imgThresh, imgThresh, cv::Size(golfGlobals::cam2BallSize, golfGlobals::cam2BallSize), 0, 0, CV_INTER_LINEAR);
			cv::threshold(imgThresh, imgThresh, golfGlobals::cam2thresh, 255, CV_THRESH_BINARY);
			cv::bitwise_and(imgThresh, cam2circleMask, imgThresh);
			pixels = cv::countNonZero(imgThresh);
			break;
		}
		else
			return 0;

	case 3:
		if(isEmptySlot == 1)
		{
			cv::threshold(image, imgThresh, golfGlobals::cam3thresh, 255, CV_THRESH_BINARY_INV);
			cv::bitwise_and(imgThresh, cam3circleMask, imgThresh);
			cv::resize(imgThresh, imgThresh, cv::Size(golfGlobals::cam3BallSize, golfGlobals::cam3BallSize), 0, 0, CV_INTER_LINEAR);
			cv::threshold(imgThresh, imgThresh, golfGlobals::cam3thresh, 255, CV_THRESH_BINARY);
			pixels = cv::countNonZero(imgThresh);
			break;
		}
		else
			return 0;

	}
	return pixels;
}

void classifyJob::findComponents(int camera, cv::Mat& image, cv::Mat& imgThresh, std::vector<classifyJob::component>& components)
{
	cv::Mat imgDilated;
	cv::dilate(imgThresh, imgDilated, dilationKernel, cv::Point(-1,-1), dil_iter);

	//#define outputImages
	#ifdef outputImages
		cv::Mat imgDisp;
		imgThresh.copyTo(imgDisp);
		cv::imshow("dialated", imgDilated);
		cv::imshow("original", image);
		cv::imshow("mask", circleMask);
	#endif

	std::vector<std::vector<cv::Point> > contours;
	cv::findContours( imgDilated, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if (contours.size() == 0)
		return;

	int idx = 0;
	for(std::vector<std::vector<cv::Point> >::iterator i = contours.begin() ; i!=contours.end(); ++i, ++idx )
	{
		cv::Mat locations, croppedImage;
		cv::Rect bounding_box_CC = cv::boundingRect(*i);

		imgThresh(bounding_box_CC).copyTo(croppedImage);
		cv::findNonZero(croppedImage, locations); //detecting logo's pixel

		if (locations.rows < rowsMin || locations.rows > rowsMax)
			continue;

		//Got a connected component so lets fit a line.
		cv::Vec4f Myline;
		cv::fitLine(locations, Myline, CV_DIST_L2, 0, 0.01, 0.01);//fitting line to logo, using opencv -> replace with own implementation

		cv::Mat residuals(locations.rows, 1, CV_32F);

		float a, b, c;
		const double tolline = 0.01;
		if(std::abs(Myline[0])>tolline){
			a = Myline[1], b = -Myline[0], c = -b*Myline[3] - a*Myline[2];
			for(int i_ind = 0; i_ind<residuals.rows; ++i_ind){
				cv::Point loc_i = locations.at<cv::Point>(i_ind);
				residuals.at<float>(i_ind) = std::fabs(a*loc_i.x+b*loc_i.y+c);
			}
		}
		else{
			a = Myline[0], b = -Myline[1], c = -b*Myline[2] - a*Myline[3];
			for(int i_ind = 0; i_ind<residuals.rows; ++i_ind){
				cv::Point loc_i = locations.at<cv::Point>(i_ind);
				residuals.at<float>(i_ind) = std::fabs(a*loc_i.y+b*loc_i.x+c);
			}
		}
		cv::Mat mean, stdvar;
		cv::meanStdDev(residuals, mean, stdvar);
		
		if(mean.at<double>(0,0) > mean_thresh || stdvar.at<double>(0,0) > tolCC_error)
			continue;

		// computing angle!
		double angle = -atan2(Myline[1],Myline[0])*180.0/CV_PI;

#ifdef outputImages
		cv::drawContours(imgDisp, contours, idx, cv::Scalar(170), 2, 8 );
		cv::Point p1(bounding_box_CC.x+Myline[2],bounding_box_CC.y+Myline[3]);
		cv::Point p2(bounding_box_CC.x+Myline[2]+Myline[0]*250,bounding_box_CC.y+Myline[3]+Myline[1]*250);
		cv::clipLine(cv::Rect(cv::Point(), imgThresh.size()), p1, p2);
		cv::line( imgDisp, p1, p2, cv::Scalar(170), 5);
#endif
		components.resize(components.size()+1);
		component& co = components.back();
		co.angle = round(angle);
		co.croppedImage = croppedImage;
		co.offset = cv::Point(bounding_box_CC.x, bounding_box_CC.y);
	}
#ifdef outputImages
	cv::imshow("imgDisp", imgDisp);
	cv::waitKey(0);
#endif
}

classifyJob::classifyJob(golfBall* b, int cam, cv::Mat& img, cv::Mat& imgThre): ball(b), camera(cam-1), image(img), imgThresh(imgThre)
{if(ball) id = ball->id;}


void classifyJobNike::classify()
{
	std::vector<std::vector<cv::Point> > contours;
	cv::Mat tmpImg = imgThresh.clone();
	cv::findContours( tmpImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for(int i=0; i<contours.size(); i++)
	{
		if(contours[i].size()>50 && contours[i].size() < 250)
		{
			double distNike = cv::matchShapes(contours[i], golfGlobals::templateContours[0], CV_CONTOURS_MATCH_I3, 0.0);
			if(distNike < 0.20)
			{
				ball->templatevotes[3]++;
				if(ball->templatevotes[3] > 5)
					ball->identified = true;
			}
		}
	}

	ball->cc_observed_times++;

	if(ball->templatevotes[3] >= 1 || ball->cc_observed_times < 15)
		return;


	std::vector<CrossCorrelationJob*> CCJobs;

	//For each connected component
	
	int checkNike = 6;
	CCJobs.reserve(components.size()*3*2);

	for(std::vector<component>::iterator i_CC = components.begin(); i_CC!=components.end(); ++i_CC)
	{
		//For each angle index
		for(int i_template = -3; i_template<=3; i_template+=3){ //templates of the same model
			int angleoff = i_CC->angle + i_template;
			int test_angles[2] = {(angleoff%360+360) % 360, ((angleoff+180)%360+360) % 360};

			//For each model
			{
				for(int i = 0; i<2; ++i)
				{
					int angle = test_angles[i];
					//Job here
					CrossCorrelationJob* CC;
					CC = new CrossCorrelationJob(Templates[checkNike][angle], i_CC->croppedImage);
					CC->Model = checkNike;
					CC->Angle = angle;
					CC->ConnectedComponent = (int)(i_CC - components.begin());

					pthread_create(&CC->thread, NULL, CrossCorrelationThread, CC);
					CCJobs.push_back(CC);
				}
			}
		}
	}

	int max_correlation_angle[1] = {0};
	double max_correlation_values[1] = {0.0};
	cv::Mat* best_CC[1] = {0};
	cv::Point max_correlation_locations[1] = {0};
	int best_model_CC = 0;
	double best_corr_CC = 0;
	int best_CC_index = 0;

	for(std::vector<CrossCorrelationJob*>::iterator i = CCJobs.begin(); i!=CCJobs.end(); ++i)
	{
		CrossCorrelationJob* job = *i;
		pthread_join(job->thread, NULL);

		if (job->maxVal > max_correlation_values[job->Model]){
			max_correlation_angle[job->Model] = job->Angle;
			max_correlation_values[job->Model] = job->maxVal;
			max_correlation_locations[job->Model] = job->maxLoc;
			best_CC[job->Model] = &job->croppedImage;
		}
		if (job->maxVal > best_corr_CC)
		{
			best_corr_CC = job->maxVal;
			best_model_CC = job->Model;
			best_CC_index = job->ConnectedComponent;
		}

		delete job;
	}
	CCJobs.clear();

	if(ball->identified) return;


	if (best_corr_CC > classifyJob::Classification_ThresholdNike)
	{
		// check ratio
		if(best_model_CC == checkNike) //NIKE
		{
			++ball->templatevotes[3];	
			if(ball->templatevotes[3]==classifyJob::Classification_Maxvotes)
			{
				ball->identified = true;
			}
			return;
		}
	}
}
		


/*new*/
void classifyJobCC::classify()
{
	if(ball->identified) return;

	std::vector<CrossCorrelationJob*> CCJobs;
	//For each connected component
	std::vector<int> checkmodelinds;
	int models[] = {0,1,2,3};
	checkmodelinds.insert(checkmodelinds.end(), models, models+4);
	int checkmodels[] = {0, 1, 2, 3};
	const int checkmodelscount = sizeof(checkmodels)/sizeof(checkmodels[0]);
	CCJobs.reserve(components.size()*3*2*checkmodelscount);
	for(std::vector<component>::iterator i_CC = components.begin(); i_CC!=components.end(); ++i_CC)
	{
		//For each angle index
		for(int i_template = -3; i_template<=3; i_template+=3){ //templates of the same model
			int angleoff = i_CC->angle + i_template;
			int test_angles[2] = {(angleoff%360+360) % 360, ((angleoff+180)%360+360) % 360};
			//For each model
			for(std::vector<int>::iterator i_model=checkmodelinds.begin(); i_model!=checkmodelinds.end(); ++i_model)
			{
				for(int i = 0; i<2; ++i)
				{
					int angle = test_angles[i];
					//Job here
					CrossCorrelationJob* CC;
					CC = new CrossCorrelationJob(Templates[checkmodels[*i_model]][angle], i_CC->croppedImage);
					CC->Model = checkmodels[*i_model];
					CC->Angle = angle;
					CC->ConnectedComponent = (int)(i_CC - components.begin());					
					pthread_create(&CC->thread, NULL, CrossCorrelationThread, CC);
					CCJobs.push_back(CC);
				}				
			}			
		}
	}
	int max_correlation_angle[checkmodelscount] = {0};
	double max_correlation_values[checkmodelscount] = {0.0};
	cv::Mat* best_CC[checkmodelscount] = {0};
	cv::Point max_correlation_locations[checkmodelscount] = {0};
	int best_model_CC = 0;
	double best_corr_CC = 0;
	int best_CC_index = 0;
	for(std::vector<CrossCorrelationJob*>::iterator i = CCJobs.begin(); i!=CCJobs.end(); ++i)
	{
		CrossCorrelationJob* job = *i;
		pthread_join(job->thread, NULL);
		if (job->maxVal > max_correlation_values[job->Model]){
			max_correlation_angle[job->Model] = job->Angle;
			max_correlation_values[job->Model] = job->maxVal;
			max_correlation_locations[job->Model] = job->maxLoc;
			best_CC[job->Model] = &job->croppedImage;
		}
		if (job->maxVal > best_corr_CC)
		{
			best_corr_CC = job->maxVal;
			best_model_CC = job->Model;
			best_CC_index = job->ConnectedComponent;
		}
		delete job;
	}
	CCJobs.clear();

	if(ball->identified) return;

	double ratioTvalue;
	double corrNX = (max_correlation_values[0]>max_correlation_values[1])? max_correlation_values[0]:max_correlation_values[1];
	double corrV1 = (max_correlation_values[2]>max_correlation_values[3])? max_correlation_values[2]:max_correlation_values[3];
	ratioTvalue = (corrNX>corrV1)?corrNX/corrV1:corrV1/corrNX;
	if (best_corr_CC > classifyJob::Classification_Threshold){
		// check ratio
		if(best_model_CC == 6) //NIKE
		{
			++ball->templatevotes[3];	
			if(ball->templatevotes[3]>=classifyJob::Classification_MaxvotesNXT)
			{
				ball->identified = true;
			}
			return;
		}
		bool cancelThreads = false;
		//custom V1 test case: V1 is evident, checking now for Prov1x
		if (ratioTvalue > classifyJob::Classification_ratioThreshold)
		{
			if (best_model_CC < 2) //NXT
			{
				if (best_corr_CC > classifyJob::Classification_Threshold_Superb)
				{
					ball->templatevotes[0]+=classifyJob::Classification_MaxvotesNXT;
					ball->identified = true;
				}
				else{
					++ball->templatevotes[0];	
					if(ball->templatevotes[0]>=classifyJob::Classification_MaxvotesNXT)
						ball->identified = true;
				}
			}
			else
			{
				bool flag = 0;
				// cropping the best location match for Pro v1
				int best_angle_CC = max_correlation_angle[best_model_CC];
				cv::Mat& croppedImage_CC = *best_CC[best_model_CC];
				cv::Size temp_size = Templates[best_model_CC][best_angle_CC].size();
				float temp_length = (float)template_w * 1.5f/2.0f + 1.15f/2.0f*(float)temp_size.width;
				float offx = cos((float)best_angle_CC/180.0f*(float)CV_PI) * temp_length;
				float offy = -sin((float)best_angle_CC/180.0f*(float)CV_PI)* temp_length;
				cv::Point2f center(
					(float)max_correlation_locations[best_model_CC].x+(float)temp_size.width/2.0f+offx,
					(float)max_correlation_locations[best_model_CC].y+(float)temp_size.height/2.0f+offy
					);
				cv::RotatedRect roiRegion = cv::RotatedRect(center, 
					cv::Size(int(template_w)*2,int(template_h)*2), 
					float(360 - best_angle_CC));
				cv::Rect bRect = roiRegion.boundingRect();
				cv::Point& offset = components[best_CC_index].offset;
				bRect.x += offset.x;
				bRect.y += offset.y;
				if ((bRect.x >= 0) && (bRect.y > 0) && (bRect.x+bRect.width < imgThresh.cols) && (bRect.y+bRect.height < imgThresh.rows))
				{
					//  successfully cropped!
					// doing template matching on this point -> recompute angles... can be optimized\\
					// cropping new bounding box of rotated rectangle
					cv::Mat croppedImage(imgThresh, bRect);
					// recomputing angles
					#ifdef outputImages
						cv::imshow("x",croppedImage);
						cv::waitKey(0);
					#endif
					for(int i_template = -3; i_template<=3; i_template+=3)
					{
						int angleoff = best_angle_CC + i_template;
						int test_angles[2] = {(angleoff%360+360) % 360, ((angleoff+180)%360+360) % 360};

						for(int i_model=4; i_model<6; i_model++)
						{							
							for(int i = 0; i<2; ++i)
							{
								int angle = test_angles[i];
								//Job here
								CrossCorrelationJob* CC;
								CC = new CrossCorrelationJob(Templates[i_model][angle], croppedImage);
								CC->Model = i_model;
								CC->Angle = angle;

								pthread_create(&CC->thread, NULL, CrossCorrelationThread, CC);
								CCJobs.push_back(CC);
							}
						}
					}
					bool cancelThreads = false;

					for(std::vector<CrossCorrelationJob*>::iterator i = CCJobs.begin(); i!=CCJobs.end(); ++i)
					{
						CrossCorrelationJob* job = *i;
						if(cancelThreads)
						{
							pthread_cancel(job->thread);
							pthread_join(job->thread, NULL);
						}
						else
						{
							pthread_join(job->thread, NULL);

							if (job->maxVal > classifyJob::Classification_Threshold)
							{
								if (job->maxVal > classifyJob::Classification_Threshold_Superb)
								{
									ball->templatevotes[2]+=classifyJob::Classification_MaxvotesV1;
									ball->identified = true;
								}
								else
								{
									ball->templatevotes[2]+=2;
									if(ball->templatevotes[2]>=classifyJob::Classification_MaxvotesV1)
										ball->identified = true;
									flag = 1;
									cancelThreads = true;
								}
							}
						}
						delete job;
					}
				}
				if(flag == 0)
				{
					if (best_corr_CC > classifyJob::Classification_Threshold_Superb)
					{
						ball->templatevotes[1]+=classifyJob::Classification_MaxvotesNXT;
						ball->V1_test = true;
					}
					else
					{
						++ball->templatevotes[1];	
						if(ball->templatevotes[1]>=classifyJob::Classification_MaxvotesNXT)
						{
							ball->V1_test = true;
							if(ball->templatevotes[1]>=classifyJob::Classification_MaxvotesV1)
								ball->identified = true;
						}
					}
				}
			}
		}
	}
	ball->cc_observed_times++;
}
void classifyJobNB::classify()
{
	if (keypoints.size() < keypoints_threshold || descriptors.rows==0 || descriptors.cols==0)
		return;
	// Compressing Test data
	cv::Mat compressed;
	compressPCA(descriptors, compressed);
	cv::Mat results, floatDescs;
	compressed.convertTo(floatDescs, CV_32FC1);

	nbc.predict(floatDescs, &results);

	++ball->observed_times;
	ball->numVotes += (int)keypoints.size();

	// Count votes for each Brand
	for(int i = 0; i<keypoints.size(); ++i)
	{
		int result = (int)results.at<float>(i, 0);
		if(result<0 || result>=golfBall::brandNames.size())
			continue;
		switch(camera)
		{
		case 0:
			++ball->cam1Votes[result];
			break;
		case 1:
			++ball->cam2Votes[result];
			break;
		case 2:
			++ball->cam3Votes[result];
			break;
		}
	}
}

bool classifyJob::compressPCA(const cv::Mat& testset, cv::Mat& compressed)
{
	// if there is no test data, just return the computed basis, ready-to-use
    if( !testset.data )
        return false;

    compressed.create(testset.rows, 90/*golfGlobals::golfData.max_compenents*/, testset.type());

    cv::Mat reconstructed;
    for( int i = 0; i < testset.rows; i++ )
    {
        cv::Mat vec = testset.row(i), coeffs = compressed.row(i);
        // compress the vector, the result will be stored
        // in the i-th row of the output matrix
        pca.project(vec, coeffs);
        // and then reconstruct it
        pca.backProject(coeffs, reconstructed);
        // and measure the error
        //printf("%d. diff = %g\n", i, norm(vec, reconstructed, NORM_L2));
    }
    return true;
}
