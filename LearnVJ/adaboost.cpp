#include "adaboost.h"
#include "features.h"
#include "../IntegralImage/integral_image.h"

#include "cv.h"
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;
using namespace cv;

const char* const base_positive = "../24/faces/";
const char* const base_negative = "../24/nonfaces/";
const char* const pos_extension = ".bmp";
const char* const neg_extension = ".bmp";

size_t LoadImage(unsigned int which_objs, unsigned int which_not_objs, 
                 vector<Mat> &pos_iis, vector<Mat> &neg_iis)
{
	char buffer[64], buffer2[128];
	size_t num_skipped = 0;
	Mat img_placeholder;
	// Get integral image of each POSITIVE sample
	for(size_t i = 0; i < which_objs; ++i)
	{
		sprintf(buffer, "face%04d", i+1);
		strcpy(buffer2, base_positive);
		strcat(buffer2, buffer);
		strcat(buffer2, pos_extension);
		img_placeholder = imread(buffer2, 0);
		if(img_placeholder.data == NULL)
		{
			cout << buffer2 << " does not exist in positive" << endl;
			num_skipped++;
			continue;
		}
		pos_iis.push_back(IntegralImage(img_placeholder)); 
	}
	// Get integral image of each NEGATIVE sample
	for(size_t i = 0; i < which_not_objs; ++i)
	{
		sprintf(buffer, "nonface%04d", i+1);
		strcpy(buffer2, base_negative);
		strcat(buffer2, buffer);
		strcat(buffer2, neg_extension);
		img_placeholder = imread(buffer2, 0);
		if(img_placeholder.data == NULL) {
			cout << buffer2 << " does not exist in negative" << endl;
			num_skipped++;
			continue;
		}
		neg_iis.push_back(IntegralImage(img_placeholder)); 
	}
	return num_skipped;
}

vector<AdaBoostFeature*> RunAdaBoost(unsigned int which_objs, unsigned int which_not_objs,
                                     unsigned int how_many, unsigned int total_set)
{
	vector<AdaBoostFeature*> afeatures;
	// Generate integral images.
	vector<Mat> pos_iis;        // 正样本积分图
	vector<Mat> neg_iis;        // 负样本积分图
	array<double> pos_weights;  // 正样本权重
	array<double> neg_weights;  // 负样本权重

	size_t num_skipped;
	num_skipped = LoadImage(which_objs, which_not_objs, pos_iis, neg_iis);
	cout << "Images are loaded. " << num_skipped << " skipped." << endl;

	// 初始化正样本权重
	// Initial weights of each POSITIVE sample  =  1/(2*n)
	for(size_t i = 0; i < pos_iis.size(); ++i)
		pos_weights.push_back((double)(1)/(double)(2 * pos_iis.size()));
	// 初始化负样本权重
	// Initial weights of each NEGATIVE sample  =  1/(2*m)
	for(size_t i = 0; i < neg_iis.size(); ++i)
		neg_weights.push_back((double)(1)/(double)(2 * neg_iis.size()));
	
	// Generate random features. 产生随机特征
	set<Feature*>* feature_set;
	if(total_set)
		feature_set = GenerateRandomFeatures(total_set);
	else
		feature_set = GenerateAllFeatures(2);
	cout << feature_set->size() << " unique features generated." << endl; 

	clock_t start;
	double minutes;
	// Run AdaBoost rounds. Get one best feature one iteration
	for(size_t i = 0; i < how_many; ++i)
	{
		start = clock();
		cout <<"======Running round "<<i<<" of Adaboost======"<< endl;
		AdaBoostFeature* best_feature = RunAdaBoostRound(pos_iis, neg_iis, pos_weights, neg_weights, feature_set);
		if(best_feature)
		{
			afeatures.push_back(best_feature);
			feature_set->erase(best_feature->feature);
		}
		else
		{
			cout<<"Can't find good feature any more"<<endl;
			break;
		}
		minutes = double(clock()-start)/CLOCKS_PER_SEC/60;
		cout << minutes << " mins. ";
		minutes *= how_many - i;
		if(minutes > 60)
			cout << (long)(minutes / 60) << " hours ";
		cout << (long)(minutes) % 60 << " mins remaining..." << endl;
	}

	return afeatures;
}

AdaBoostFeature* RunAdaBoostRound(const vector<Mat> &pos_iis, const vector<Mat> &neg_iis,
                                  array<double> &pos_weights, array<double> &neg_weights,
                                  const set<Feature*> *feature_set)
{
	/** Step 1. Normalize the weights 归一化权重，所有正负样本 */
	cout << "Normalizing Weights..." << endl;
	double weights_sum = 0;
	// calculate sum of positive and negative sample weights
	for(size_t i = 0; i < pos_weights.size(); ++i)
		weights_sum += pos_weights[i];
	for(size_t i = 0; i < neg_weights.size(); ++i)
		weights_sum += neg_weights[i];
	// do normalize
	for(size_t i = 0; i < pos_weights.size(); ++i)
		pos_weights[i] /= weights_sum;
	for(size_t i = 0; i < neg_weights.size(); ++i)
		neg_weights[i] /= weights_sum;

	/** Step 2. Find the feature with the best error */
	// 找到错误率最小的特征（弱分类器）
	cout << "Finding best feature..." << endl;
	int cur_threshold, cur_polarity;
	int best_threshold = 0, best_polarity = 0;
	double best_error, cur_error;
	double best_false_pos_rate, cur_false_pos_rate;
	Feature* best_feature = NULL;
	best_error = numeric_limits<double>::infinity();
	best_false_pos_rate = numeric_limits<double>::infinity();
	// 对每个特征训练一个弱分类器，计算其加权错误率。找到错误率最小的。
	// for each random feature, find a *single* best feature
	set<Feature*>::const_iterator feature_it;
	for(feature_it = feature_set->begin(); feature_it != feature_set->end(); ++feature_it)
	{
		if((*feature_it)->positive_results.empty())
			(*feature_it)->calculate_pos_results(pos_iis);
		if((*feature_it)->negative_results.empty())
			(*feature_it)->calculate_neg_results(neg_iis);
		// 找出当前弱分类器的阈值，极性和错误率。
		FindThresholdAndPolarity((*feature_it)->positive_results,
		                         (*feature_it)->negative_results,
		                         pos_weights, neg_weights,
		                         &cur_threshold, &cur_polarity,
		                         &cur_error, &cur_false_pos_rate);
		// update best error 找到最小迭代误差之和的样本
		if(cur_error < best_error)
		{
			best_error = cur_error;
			best_threshold = cur_threshold;
			best_polarity = cur_polarity;
			best_false_pos_rate = cur_false_pos_rate;
			best_feature = *feature_it;
		}
	}
	// 错误率大于0.5则舍弃
	if(best_error > 0.5)
		return NULL;
	cout << "Weighted error rate " << best_error << endl;
	cout << "False positive rate " << best_false_pos_rate << endl;
	
	/** Step 3. Update the weights */
	cout << "Updating weights... ";
	// 更新正/负样本权重
	double beta = (best_error)/(1 - best_error);
	cout << "beta = " << beta << endl;
	// use beta to update weights of each Positive and Negative feature
	// 如果正/负样本可以被正确分类，则更新该*样本*的权重
	const array<int> &positive_results = best_feature->positive_results;
	const array<int> &negative_results = best_feature->negative_results;
	assert(positive_results.size() == pos_weights.size());
	for(size_t i = 0; i < positive_results.size(); ++i)
	{
		// Correctly identified as true, reduce weight. Else leave the same
		if(best_polarity * positive_results[i] < best_polarity * best_threshold)
			pos_weights[i] *= beta;
	}
	assert(negative_results.size() == neg_weights.size());
	for(size_t i = 0; i < negative_results.size(); ++i)
	{
		// Correctly identified as false, reduce weight. Else leave the same
		if(best_polarity * negative_results[i] >= best_polarity * best_threshold)
			neg_weights[i] *= beta;
	}
	
	/** Return best feature */
	AdaBoostFeature* result = new AdaBoostFeature();
	result->feature = best_feature;
	result->threshold = best_threshold;
	result->polarity = best_polarity;
	result->beta_t = beta;
	result->false_pos_rate = best_false_pos_rate;
	return result;
}

// Try every example value as the threshold. This is WLOG.
// 找出某个弱分类器的阈值，极性和错误率。
void FindThresholdAndPolarity(const array<int> &positive_results, const array<int> &negative_results, 
                              const array<double> &pos_weights, const array<double> &neg_weights,
                              int* threshold, int* polarity, double* error, double* false_pos_rate)
{
	size_t total_samples;
	int best_threshold = 0;
	int best_polarity = 0;
	double best_error = 0;
	double best_false_pos_sum; // 假阳性数
	double pos_pol_err_sum, neg_pol_err_sum;
	double false_pos_sum1, false_pos_sum2;
	// 初始化为无穷大
	best_error = numeric_limits<double>::infinity();
	best_false_pos_sum = numeric_limits<double>::infinity();
	// 论文中为加权错误率=w*|h-y|，当正确分类时|h-y|=0
	// for weak classifier of each sample, update threshold, polarity and error
	total_samples = positive_results.size() + negative_results.size();
	for(size_t its = 0; its < total_samples; its++)
	{
		pos_pol_err_sum = 0; // 作为*正*特征的加权错误率之和 S+
		neg_pol_err_sum = 0; // 作为*负*特征的加权错误率之和 N+
		int cur_threshold;
		if(its < positive_results.size())
			cur_threshold = positive_results[its]; // <= as a threshold
		else
			cur_threshold = negative_results[its-positive_results.size()];
		// 计算当前*样本权值*作为*阈值*的错误率
		for(size_t i = 0; i < positive_results.size(); ++i)
		{
			// Misclasified with polarity of 1
			if(!(positive_results[i]*1 < cur_threshold*1))
				pos_pol_err_sum += pos_weights[i];
			// Misclassified with polarity of -1
			if(!(positive_results[i]*-1 < cur_threshold*-1))
				neg_pol_err_sum += pos_weights[i];
		}
		false_pos_sum1 = 0;
		false_pos_sum2 = 0;
		for(size_t i = 0; i < negative_results.size(); ++i)
		{
			// Misclasified with polarity of 1
			if(!(negative_results[i]*1 >= cur_threshold*1))
			{
				pos_pol_err_sum += neg_weights[i];
				false_pos_sum1++;
			}
			// Misclassified with polarity of -1
			if(!(negative_results[i]*-1 >= cur_threshold*-1))
			{
				neg_pol_err_sum += neg_weights[i];
				false_pos_sum2++;
			}
		}
		if(pos_pol_err_sum < best_error)
		{
			best_threshold = cur_threshold;
			best_polarity = 1;
			best_error = pos_pol_err_sum;
			best_false_pos_sum = false_pos_sum1;
		}
		if(neg_pol_err_sum < best_error)
		{
			best_threshold = cur_threshold;
			best_polarity = -1; 
			best_error = neg_pol_err_sum;
			best_false_pos_sum = false_pos_sum2;
		}
	}
	*threshold = best_threshold;
	*polarity = best_polarity;
	*error = best_error;
	*false_pos_rate = best_false_pos_sum / negative_results.size();
}

void SaveAdaBoost(const vector<AdaBoostFeature*> to_save, const char* filename)
{
	ofstream save_file;
	save_file.open(filename);
	save_file << SUBWINDOW_SIZE << endl;
	save_file << to_save.size() << endl;
	vector<AdaBoostFeature*>::const_iterator it;
	for(it = to_save.begin(); it != to_save.end(); ++it)
	{
		save_file << (*it)->feature->type << " " <<
		(*it)->feature->x1 << " " << (*it)->feature->y1 << " " <<
		(*it)->feature->x2 << " " << (*it)->feature->y2 << " " <<
		(*it)->threshold << " " << (*it)->polarity << " " <<
		(*it)->beta_t << " " << (*it)->false_pos_rate << " " <<
		"\n";
	}
	save_file.close();
}

