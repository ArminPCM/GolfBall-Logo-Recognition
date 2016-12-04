#include "BrandDecisionRule.h"

DR_thresholdsPerCase::DR_thresholdsPerCase(int id, float rp_same, float rp_diff, float rp_factor, float vp, int nv1, int nv2)
{
	caseID = id;
	ratio_percentage_sameCam = rp_same; 
	ratio_percentage_DiffCam = rp_diff;
	ratio_percentage_factor = rp_factor;	
	percentage_votes = vp;
	num_votes = nv1;	
	num_votes_2nd_peak = nv2;
}


DR_thresholdsPerCase DR_thresholds::CreateCase(int id)
{
	DR_thresholdsPerCase newcase;
	switch (id) {
		// votes > threshold_split1 + 3 aligned peaks
	case 1: 
		newcase = DR_thresholdsPerCase(1, 1.4, 0.0, 0.8, 0.0, 500, 0);
		break;
		// votes > threshold_split1 + 2 aligned peaks
	case 2:
		newcase = DR_thresholdsPerCase(2, 1.35, 1.35, 0.9, 0.0, 0, 0);
		break;
		// votes > threshold_split1 + no aligned peaks
	case 3:
		newcase = DR_thresholdsPerCase(3, 1.5, 1.5, 0.8, 0.0, 500, 0);
		break;
		// votes > threshold_split1 + ambiguos
	case 4:
		newcase = DR_thresholdsPerCase(4, 0.0, 0.0, 0.0, 0.0, 0, 0);
		break;
		// threshold_split2 < votes < threshold_split1 + 3 aligned peaks
	case 5:
		newcase = DR_thresholdsPerCase(5, 2.0, 0.0, 0.8,  0.3, 0, 0);
		break;
		// threshold_split2 < votes < threshold_split1 + 2 aligned peaks
	case 6:
		newcase = DR_thresholdsPerCase(6, 2.0, 3.5, 0.8, 0.3, 0, 50);
		break;
		// threshold_split2 < votes < threshold_split1 + no aligned peaks
	case 7:
		newcase = DR_thresholdsPerCase(7, 2.0, 3.5, 0.8, 0.3, 0, 0);
		break;
		// threshold_split2 < votes < threshold_split1 + ambiguos
	case 8:
		newcase = DR_thresholdsPerCase(8, 0.0, 0.0, 0.0, 0.0, 0, 0);
		break;
		// votes < threshold_split2 + 3 aligned peaks
	case 9:
		newcase = DR_thresholdsPerCase(9, 2.0, 0.0, 0.8, 0.3, 0, 0);
		break;
		// votes < threshold_split2 + 2 aligned peaks
	case 10:
		newcase = DR_thresholdsPerCase(10, 2.0, 3.5, 0.8, 0.3, 0, 50);
		break;
	default: 
		newcase = DR_thresholdsPerCase();
		break;
	}

	return newcase;
}

C_CameraData::C_CameraData(std::vector<int> &camera, int idn){
	id =  idn;

	int sumvotes = 0;
	std::vector<int>::iterator it = camera.begin();
	std::vector<int>::iterator it_ind1_local = it , it_ind2_local = it; 

	for(++it; it!=camera.end(); ++it)
	{
		sumvotes += *it;
		if ( *it>*it_ind1_local) // this is useful to compute the two largest elements and their indices
		{
			it_ind2_local = it_ind1_local;
			it_ind1_local = it;
		}
		else
		{ 
			if(*it>*it_ind2_local)
				it_ind2_local = it;
		}
	}

	// checks if camera is "active" or "inactive" (e.g., logo only visible from 1 camera) then sumvotes (and all stats) is 0.
	if (sumvotes)
	{
		total_votes = sumvotes;
		float total_votesf = (float)sumvotes;
		// maximum values
		max_peak = *it_ind1_local;
		max_subpeak = *it_ind2_local;
		// indices peaks
		ind_peak = it_ind1_local - camera.begin();
		ind_subpeak = it_ind2_local - camera.begin();
		// computations
		ratio_peak_subpeak = (ind_subpeak>0)? max_peak/(float)max_subpeak: (float)max_peak;  // if subpeak is 0 (e.g., logo only visible from 1 camera) then the ratio  is very large but no NAN
		percentage_votes_peak = max_peak/total_votesf;
		percentage_votes_subpeak = max_subpeak/total_votesf;
		// iterators
		it_ind1 = it_ind1_local;
		it_ind2 = it_ind2_local;
		it_begin = camera.begin();
		it_end = camera.end();
	}
	else
	{
		total_votes = 0;
		// maximum values
		max_peak = 0;
		max_subpeak = 0;
		// indices peaks
		ind_peak = 0;
		ind_subpeak = 0;
		// computations
		ratio_peak_subpeak = 0;  // if subpeak is 0 (e.g., logo only visible from 1 camera) then the ratio  is very large but no NAN
		percentage_votes_peak = 0;
		percentage_votes_subpeak = 0;
		// iterators
		it_ind1 = camera.end();
		it_ind2 = camera.end();
		it_begin = camera.begin();
		it_end = camera.end();
	}
}

C_DR_thresholds::C_DR_thresholds()
{
	// by defaul there are 11 cases, but only 9 require thresholds	
	for(int i=1; i<=10; i++)// Creating all thresholds for all cases
		thresholds_cases.push_back(CreateCase(i));

	split_root = 500;
	split_lvl1 = 200;
}

void BrandDecisionRule::ImportData(std::vector<int>&vec1,std::vector<int>&vec2,std::vector<int>&vec3)
{
	cameras.push_back(CameraData(vec1, 1));
	cameras.push_back(CameraData(vec2, 2));
	cameras.push_back(CameraData(vec3, 3));
	CrossCameraData();
	NoRead = 0;
}

void BrandDecisionRule::CrossCameraData()
{
	// sorting the cameras (fast... 3 comparisons)
	std::vector<CameraData>::iterator it = cameras.begin();
	// replace this by partial sort...no need to keep original order of cameras... only for debugging
	if (it->max_peak > (it+1)->max_peak) 
	{ 
		if (it->max_peak > (it+2)->max_peak)
		{
			it_sorted.push_back(it);
			if ((it+1)->max_peak > (it+2)->max_peak) // it >it+1>it+2
			{
				it_sorted.push_back(it+1);
				it_sorted.push_back(it+2);
			}
			else									// it >it+2>it+1
			{
				it_sorted.push_back(it+2);
				it_sorted.push_back(it+1);
			}		
		}
		else
		{
			it_sorted.push_back(it+2);				// it+2 >it>it+1
			it_sorted.push_back(it);
			it_sorted.push_back(it+1);			
		}
	}
	else
	{
		if ((it+1)->max_peak > (it+2)->max_peak)
		{
			it_sorted.push_back((it+1));
			if ((it)->max_peak > (it+2)->max_peak) // it+1>it>it+2
			{
				it_sorted.push_back(it);
				it_sorted.push_back(it+2);
			}
			else									// it+1>it+2>it
			{
				it_sorted.push_back(it+2);
				it_sorted.push_back(it);
			}		
		}
		else
		{
			it_sorted.push_back(it+2);				// it+2 >it+1>it
			it_sorted.push_back((it+1));
			it_sorted.push_back(it);
		}

	}
	std::vector<std::vector<CameraData>::iterator>::iterator it_rep = it_sorted.begin();
	// comparing which peaks are the same than the best peak - change wrt matlab's code
	it_same.push_back((*it_rep));
	peaks_aligned = 1;
	peaks0 = 0;
	// second best
	if ((*(it_rep+1))->total_votes)
	{
		if ( (*it_rep)->ind_peak == (*(it_rep+1))->ind_peak )
		{
			it_same.push_back(*(it_rep+1));
			++peaks_aligned;
		}
		else
			if((*(it_rep+1))->total_votes)
				it_diff.push_back((*(it_rep+1)));
	}
	else
		++peaks0;
	// third best
	if ((*(it_rep+2))->total_votes)
	{
		if ((*it_rep)->ind_peak == (*(it_rep+2))->ind_peak)
		{
			it_same.push_back((*(it_rep+2)));
			++peaks_aligned;
		}
		else
			it_diff.push_back(*(it_rep+2));
	}
	else
		++peaks0;
	peaks_aligned = it_same.size();

	// cumulative ratio (over all cameras) between 2 peak brands from different cameras
	int cumvotes_bestpeak = 0, cumvotes_diff_2ndpeak = 0;;
	// it is always greater than 1
	for (std::vector<std::vector<CameraData>::iterator>::iterator it_aux= it_same.begin(); it_aux!=it_same.end(); ++it_aux)
		cumvotes_bestpeak += *(((*it_aux)->it_begin)+it_same[0]->ind_peak);
	if(peaks_aligned+peaks0<3)
		for (std::vector<std::vector<CameraData>::iterator>::iterator it_aux= it_same.begin(); it_aux!=it_same.end(); ++it_aux)
			cumvotes_diff_2ndpeak += *(((*it_aux)->it_begin)+it_diff[0]->ind_peak);
	//
	if(cumvotes_diff_2ndpeak)
	{
		cumulative_ratio_diff = cumvotes_bestpeak/(float)cumvotes_diff_2ndpeak;
		ratio_diff = it_same[0]->max_peak/(float)it_diff[0]->max_peak;
	}
	else
	{
		cumulative_ratio_diff = (float)cumvotes_bestpeak;
		ratio_diff = (float)it_same[0]->max_peak;
	}
	/*	// cumulative ratio (over all cameras) between 2 peak brands from different cameras
	int cumvotes_diff_bestpeak = 0;
	// it is always greater than 1
	for (std::vector<std::vector<CameraData>::iterator>::iterator it_aux= it_same.begin(); it_aux!=it_same.end(); ++it_aux)
	{
	if((*it_aux)->total_votes>0)
	cumvotes_diff_bestpeak += *(((*it_aux)->it_begin)+it_same[0]->ind_peak);
	}
	if (peaks_aligned < 3) // means that there is at least 1 peak different 
	{
	int cumvotes_diff_2ndpeak = 0;
	for (std::vector<std::vector<CameraData>::iterator>::iterator it_aux= it_diff.begin(); it_aux!=it_diff.end(); ++it_aux)
	{
	if((*it_aux)->total_votes>0)
	cumvotes_diff_2ndpeak += *(((*it_aux)->it_begin)+it_diff[0]->ind_peak);
	}
	cumulative_ratio_diff = cumvotes_diff_bestpeak/(float)cumvotes_diff_2ndpeak;
	}
	else
	{
	cumulative_ratio_diff = (float)cumvotes_diff_bestpeak;
	}
	*/
	// cumulative ratio (over all cameras) between 2 peak brands from the same camera
	int cumvotes_same_bestpeak = 0, cumvotes_same_subpeak = 0;
	for (it = cameras.begin(); it!=cameras.end(); ++it)
	{
		cumvotes_same_bestpeak += *((it->it_begin)+it_same[0]->ind_peak);
		cumvotes_same_subpeak  += *((it->it_begin)+it_same[0]->ind_subpeak);
	}
	if(cumvotes_same_subpeak)
		cumulative_ratio_same = cumvotes_same_bestpeak/(float)cumvotes_same_subpeak;
	else
		cumulative_ratio_same = (float)cumvotes_same_bestpeak;
	
	cumulative_sum_peak = cumvotes_same_bestpeak;
}

// votes(peak) > split1 && 3 aligned peaks
bool BrandDecisionRule::f_decision1()
{
	DR_thresholdsPerCase thd = thresholds.thresholds_cases[0];
	if(peaks_aligned < 3)
		return false;
	if ( it_sorted[0]->ratio_peak_subpeak > thd.ratio_percentage_sameCam)
		return true;
	else
	{
		if(cumulative_ratio_same > thd.ratio_percentage_sameCam)
			return true;
		else
			if((cumulative_ratio_same>thd.ratio_percentage_sameCam*thd.ratio_percentage_factor)&&(cumulative_sum_peak > thd.num_votes))
				return true;
	}

	return false;
}
// Case 2: votes(peak) > threshold && 2 aligned peaks (including maximum peak)
bool BrandDecisionRule::f_decision2()
{

	DR_thresholdsPerCase thd = thresholds.thresholds_cases[1];
	if(peaks_aligned != 2) //only for maxim peak with 2 elements
		return false;

	if ( (it_sorted[0]->ratio_peak_subpeak > thd.ratio_percentage_sameCam)&&(ratio_diff>thd.ratio_percentage_DiffCam))
		return true;
	else
	{
		if(cumulative_ratio_same > thd.ratio_percentage_sameCam*thd.ratio_percentage_factor)
			return true;
	}

	return false;
}

// Case 3: votes(peak) > threshold && NO aligned peaks
bool BrandDecisionRule::f_decision3()
{
	DR_thresholdsPerCase thd = thresholds.thresholds_cases[2];

	if(peaks_aligned > 1) //only for maxim peak with 1 element
		return false;
	
	if ((cumulative_ratio_same > thd.ratio_percentage_sameCam)&&(ratio_diff > thd.ratio_percentage_DiffCam))
		return true;
	else
	{
		if((cumulative_ratio_diff>thd.ratio_percentage_factor*thd.ratio_percentage_DiffCam)&&(cumulative_sum_peak>thd.num_votes))
			return true;
	}

	return false;
}
// Case 5: votes(peak) < threshold && 3 aligned peaks
bool BrandDecisionRule::f_decision5()
{
	DR_thresholdsPerCase thd = thresholds.thresholds_cases[4];
	if ( (peaks_aligned == 3)&&(it_sorted[0]->percentage_votes_peak > thd.percentage_votes))
	{
		//case 1:
		if(it_sorted[0]->ratio_peak_subpeak>thd.ratio_percentage_sameCam) //distinctive peak
			return true;
		//case 2: - checkif make sense
		else if( (it_sorted[0]->ratio_peak_subpeak>thd.ratio_percentage_sameCam*thd.ratio_percentage_factor)&&(it_sorted[1]->ratio_peak_subpeak>thd.ratio_percentage_sameCam*thd.ratio_percentage_factor)&&(it_sorted[2]->ratio_peak_subpeak>thd.ratio_percentage_sameCam*thd.ratio_percentage_factor) )
			return true;
	}	

	return false;
}

// Case 6: votes(peak) < threshold && 2 aligned peaks
bool BrandDecisionRule::f_decision6()
{
	DR_thresholdsPerCase thd = thresholds.thresholds_cases[5];
	if (peaks_aligned != 2)
            return false;

	if( (it_sorted[0]->percentage_votes_peak > thd.percentage_votes)&&(ratio_diff > thd.ratio_percentage_DiffCam))
		return true;
	else
	{
		// ratio between best 2 peaks (even if they are the same brand)
		float adj_thd = thd.ratio_percentage_factor*thd.ratio_percentage_sameCam;
		if((it_sorted[0]->ratio_peak_subpeak > adj_thd)&&(it_same[1]->ratio_peak_subpeak>adj_thd)&&(it_same[1]->max_peak>thd.num_votes_2nd_peak))
			return true;
	}

	return false;
}
// Case 7: votes(peak) > threshold && NO aligned peaks
bool BrandDecisionRule::f_decision7()
{
	DR_thresholdsPerCase thd = thresholds.thresholds_cases[6];
	if (peaks_aligned > 1)
            return false;
	if(ratio_diff>thd.ratio_percentage_DiffCam)
	{
		if(it_sorted[0]->ratio_peak_subpeak>thd.ratio_percentage_sameCam)
			return true;
		else
			if((it_sorted[0]->ratio_peak_subpeak>thd.ratio_percentage_sameCam*thd.ratio_percentage_factor)&&(it_sorted[0]->percentage_votes_peak>thd.percentage_votes))
				return true;
	}

	return false;
}

// Case 9: votes(peak) < 200 && 3 aligned peaks - NO CASES YET!!!
bool BrandDecisionRule::f_decision9()
{
	DR_thresholdsPerCase thd = thresholds.thresholds_cases[8];

	if ((peaks_aligned == 3)&&(it_sorted[0]->percentage_votes_peak > thd.percentage_votes))
	{
		if(it_sorted[0]->ratio_peak_subpeak > thd.ratio_percentage_sameCam)
			return true;
		float adj_thd = thd.ratio_percentage_factor*thd.ratio_percentage_sameCam;
		if((it_sorted[0]->ratio_peak_subpeak > adj_thd)&&(it_sorted[1]->ratio_peak_subpeak>adj_thd)&&(it_sorted[2]->ratio_peak_subpeak>adj_thd))
			return true;
	}

	return false;
}

bool BrandDecisionRule::f_decision10()
{
	DR_thresholdsPerCase thd = thresholds.thresholds_cases[9];

	if(peaks_aligned != 2)
		return false;

	if((it_sorted[0]->percentage_votes_peak > thd.percentage_votes>thd.percentage_votes)&&(ratio_diff>thd.ratio_percentage_DiffCam))
		return true;
	float adj_thd = thd.ratio_percentage_factor*thd.ratio_percentage_sameCam;
	if((it_sorted[0]->percentage_votes_peak>adj_thd)&&(it_same[1]->ratio_peak_subpeak>adj_thd)&&(it_same[1]->max_peak>thd.num_votes))
		return true;

	return false;
}

//main rule
int BrandDecisionRule::decision_rule()
{
	int NoRead = 9; // NoRead
	if(it_sorted[0]->max_peak > thresholds.split_root)
	{
		// case 1
		if(f_decision1()) 
			return (it_sorted[0]->ind_peak);
		//case 2
		if(f_decision2()) 
			return (it_sorted[0]->ind_peak);
		//case 3
		if(f_decision3()) 
			return (it_sorted[0]->ind_peak);
		//case 4
		return NoRead;
	}
	else
	{
		if(it_sorted[0]->max_peak > thresholds.split_lvl1)
		{
			// case 5
			if(f_decision5()) 
				return (it_sorted[0]->ind_peak);
			//case 6
			if(f_decision6()) 
				return (it_sorted[0]->ind_peak);
			//case 7
			if(f_decision7()) 
				return (it_sorted[0]->ind_peak);
			//case 8
			return NoRead;
		}
		else
		{
			//case 9
			if(f_decision9()) 
				return (it_sorted[0]->ind_peak);
			//case 10
			if(f_decision10()) 
				return (it_sorted[0]->ind_peak);
			//case 11
			return NoRead;
		}
	}
	
	return NoRead;
}

void BrandDecisionRule::clear()
{
	cameras.clear();
	it_sorted.clear();
	peaks_aligned = 0;
	it_same.clear();
	it_diff.clear();
	cumulative_sum_peak = 0;
	ratio_diff = 0;
	cumulative_ratio_diff = 0.0;
	cumulative_ratio_same = 0.0;
	peaks0 = 0;
}