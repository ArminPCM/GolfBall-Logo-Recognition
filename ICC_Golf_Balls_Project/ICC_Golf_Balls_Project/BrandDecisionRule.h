#pragma once
#include <iostream>
#include <vector>
#include <algorithm> 


#ifndef BRANDDECISIONRULE.H

// struct with all constraints (possible) for a given case
struct DR_thresholdsPerCase{
// all possible constraints for all cases
	int caseID;
	// ratio between the best 2 brands in the same camera	
	float ratio_percentage_sameCam; 
	// ratio between the best 2 different brands in two different cameras.
	float ratio_percentage_DiffCam;
	// factor to relax the ration_percentage under certain circumstances.
	float ratio_percentage_factor;
	// min porcentage of votes that the brand should achieve for the best camera
	float percentage_votes;
	// min porcentage of votes that the brand should achieve for the best camera
	int num_votes;
	// min porcentage of votes that the brand should achieve for the second best camera
	int num_votes_2nd_peak;

	DR_thresholdsPerCase(){};
	DR_thresholdsPerCase(int, float, float, float, float, int, int);
};

typedef class C_DR_thresholds{
	// variables to traverse the tree based on the number of votes
public:
	int split_root;
	int split_lvl1;
	// vector with  the thresholds for the different cases
	std::vector<DR_thresholdsPerCase> thresholds_cases;
	// intialize the structure with the standard values (hardcoded)
	C_DR_thresholds();
	DR_thresholdsPerCase CreateCase(int);
	//* ADD methods to manipulate the addding/removal of new cases *\\

}DR_thresholds;


typedef class C_CameraData{
public:
	// camera number
	int id;
	// number of total votes
	int total_votes;
	// index of the maximum element (brand)
	int ind_peak;
	// index of the second maximum element (brand)
	int ind_subpeak;
	// max element (# votes)
	int max_peak;
	// second max element (# votes)
	int max_subpeak;
	// ratio between best and second best
	double ratio_peak_subpeak;
	// percentage of votes for peak
	double percentage_votes_peak;
	// percentage of votes for sub-peak
	double percentage_votes_subpeak;

	// auxiliar: 
	// iterators for the best indices
	std::vector<int>::iterator it_ind1;
	std::vector<int>::iterator it_ind2;
	std::vector<int>::iterator it_begin;
	std::vector<int>::iterator it_end;

	C_CameraData(){};
	C_CameraData(std::vector<int>&, int);

}CameraData;


typedef class C_BrandDecisionRule
{
public: 
	DR_thresholds thresholds;

	int NoRead;
	std::vector<CameraData> cameras;
	// Cross camera data \\
	// sorted cameras
	std::vector<std::vector<CameraData>::iterator> it_sorted;
	// number of peaks from common brands
	int peaks_aligned, peaks0;
	// same and different peaks
	std::vector<std::vector<CameraData>::iterator> it_same;
	std::vector<std::vector<CameraData>::iterator> it_diff;
	int cumulative_sum_peak;
	float ratio_diff;
	// cumulative ratio (over all cameras) between 2 peak brands from different cameras
	float cumulative_ratio_diff;
	// cumulative ratio (over the cameras aligned with the largest peak) between 2 peak brands from the same camera
	float cumulative_ratio_same;

	//methods
	C_BrandDecisionRule():thresholds(){};
	void ImportData(std::vector<int>&,std::vector<int>&,std::vector<int>&);
	void CrossCameraData();
	// cases
	bool f_decision1();
	bool f_decision2();
	bool f_decision3();
	bool f_decision5();
	bool f_decision6();
	bool f_decision7();
	bool f_decision9();
	bool f_decision10();

	//main rule
	int decision_rule();
	void clear();

}BrandDecisionRule;

#endif