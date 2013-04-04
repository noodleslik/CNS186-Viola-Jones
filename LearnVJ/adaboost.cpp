#include "adaboost.h"
#include "features.h"
#include "../IntegralImage/integral_image.h"

#include "cv.h"
#include "highgui.h"
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <omp.h>

using namespace std;
using namespace cv;

const char* const base_positive = "../Faces_Normalized/";
const char* const base_negative = "../Negative_Dataset/";
const char* const extension = ".jpg";
const char* const extension2 = ".jpg";

size_t LoadImage(unsigned int which_faces, unsigned int which_not_faces, 
                 vector<Mat> &pos_iis, vector<Mat> &neg_iis)
{
	size_t i, num_skipped = 0;
	Mat img_placeholder;
	// Get integral image of each POSITIVE sample
	for(i=0; i < which_faces; ++i)
	{
		char buffer[7];
		sprintf(buffer, "%d", i+1);
		char buffer2[100];
		strcpy(buffer2, base_positive);
		strcat(buffer2, buffer);
		strcat(buffer2, extension);
		img_placeholder = imread(buffer2, 0);
		if(img_placeholder.data == NULL)
		{
			cout << "Image number " << i+1 << " does not exist in positive examples. Skipping." << endl;
			num_skipped++;
			continue;
		}
		pos_iis.push_back(IntegralImage(img_placeholder)); 
	}
	// Get integral image of each NEGATIVE sample
	for(i=0; i < which_not_faces; ++i)
	{
		char buffer[7];
		sprintf(buffer, "%d", i+1);
		char buffer2[100];
		strcpy(buffer2, base_negative);
		strcat(buffer2, buffer);
		strcat(buffer2, extension2);
		img_placeholder = imread(buffer2, 0);
		if(img_placeholder.data == NULL) {
			cout << "Image number " << i+1 << " does not exist in negative examples. Skipping." << endl;
			num_skipped++;
			continue;
		}
		neg_iis.push_back(IntegralImage(img_placeholder)); 
	}
	return num_skipped;
}

vector<AdaBoostFeature*> RunAdaBoost(unsigned int which_faces, unsigned int which_not_faces,
                                     unsigned int how_many, unsigned int total_set)
{
	size_t i;
	vector<AdaBoostFeature*> container;
	// Generate integral images.
	vector<Mat> pos_iis;        // 正样本积分图
	vector<double> pos_weights; // 正样本权重
	vector<Mat> neg_iis;        // 负样本积分图
	vector<double> neg_weights; // 负样本权重

	size_t num_skipped;
	num_skipped = LoadImage(which_faces, which_not_faces, pos_iis, neg_iis);
	cout << "All images are loaded. " << num_skipped << " images skipped." << endl;

	// 初始化正样本权重
	// Initial weights of each POSITIVE sample  =  1/(2*n)
	for(i=0; i < pos_iis.size(); ++i)
	{
		pos_weights.push_back((double)(1)/(double)(2 * pos_iis.size()));
	}
	// 初始化负样本权重
	// Initial weights of each NEGATIVE sample  =  1/(2*m)
	for(i=0; i < neg_iis.size(); ++i)
	{
		neg_weights.push_back((double)(1)/(double)(2 * neg_iis.size()));
	}
	
	// Generate random features. 产生随机特征
	vector<Feature*>* feature_set;
	if(total_set)
		feature_set = GenerateRandomFeatures(total_set);
	else
		feature_set = GenerateAllFeatures(2);
	cout << feature_set->size() << " unique features generated." << endl; 

	clock_t start;
	double minutes;
	// Run AdaBoost rounds. Get one best feature one iteration
	for(i=0; i < how_many; ++i)
	{
		start = clock();
		cout <<"======Running round "<<i<<" of Adaboost procedure.======"<< endl;
		AdaBoostFeature* best_feature = RunAdaBoostRound(pos_iis, neg_iis, pos_weights, neg_weights, feature_set);
		if(best_feature)
		{
			container.push_back(best_feature);
			feature_set->erase(feature_set->begin() + best_feature->feature_id);
		}
		else
		{
			cout<<"Can't find good feature any more"<<endl;
			break;
		}
		minutes = double(clock()-start)/CLOCKS_PER_SEC/60;
		cout<<"Use "<<minutes<<" minutes."<<endl;
		minutes *= how_many-i;
		if(minutes > 60)
			cout<<(long)(minutes / 60)<<" hours ";
		cout<<(long)(minutes) % 60<<" minutes Remaining..."<<endl;
	}

	return container;
}

AdaBoostFeature* RunAdaBoostRound(const vector<Mat> &pos_iis, const vector<Mat> &neg_iis,
                                  vector<double> &pos_weights, vector<double> &neg_weights,
                                  vector<Feature*> *feature_set)
{
	/** Step 1. Normalize the weights 归一化权重，所有正负样本 */
	cout << "Normalizing Weights..." << endl;
	double weight_sums = 0;
	vector<double>::iterator it;
	// calculate sum of positive and negative sample weights
	for(it = pos_weights.begin(); it != pos_weights.end(); ++it)
		weight_sums += *it;
	for(it = neg_weights.begin(); it != neg_weights.end(); ++it)
		weight_sums += *it;
	// do normalize
	for(it = pos_weights.begin(); it != pos_weights.end(); ++it)
		*it /= weight_sums;
	for(it = neg_weights.begin(); it != neg_weights.end(); ++it)
		*it /= weight_sums;

	/** Step 2. Find the feature with the best error */
	// 找到错误率最小的特征（弱分类器）
	cout << "Finding best feature..." << endl;
	int cur_threshold, cur_polarity;
	int best_threshold = 0, best_polarity = 0;
	double best_error, cur_error;
	Feature* best_feature = NULL;
	best_error = numeric_limits<double>::infinity();// init best error infinit
	vector<Mat>::const_iterator im_it;
	vector<int> positive_results; // feature value of positive sample 
	vector<int> negative_results; // feature value of negative sample
	size_t best_feature_i = 0, which_feature = 0;
	// 对每个特征训练一个弱分类器，计算其加权错误率。找到错误率最小的。
	// for each random feature, find a *single* best feature
	size_t total_features = feature_set->size();
	//#pragma omp parallel for
	for(which_feature = 0; which_feature < total_features; which_feature++)
	{
		if(which_feature % 1000 == 0)
			cout << "Calculating feature " << which_feature << endl;
		
		positive_results.clear();
		negative_results.clear();
		// calcualte feature's value of each positive sample
		for(im_it = pos_iis.begin(); im_it != pos_iis.end(); ++im_it)
			positive_results.push_back(CalculateFeature(feature_set->at(which_feature), *im_it));
		// calcualte feature's value of each negative sample
		for(im_it = neg_iis.begin(); im_it != neg_iis.end(); ++im_it)
			negative_results.push_back(CalculateFeature(feature_set->at(which_feature), *im_it));
		
		// 找出当前弱分类器的阈值，极性和错误率。
		FindThresholdAndPolarity(positive_results, negative_results,
		                         pos_weights, neg_weights,
		                         &cur_threshold, &cur_polarity,
		                         &cur_error);
		// update best error 找到最小迭代误差之和的样本
		if(cur_error < best_error)
		{
			best_error = cur_error;
			best_threshold = cur_threshold;
			best_polarity = cur_polarity;
			best_feature = feature_set->at(which_feature);
			best_feature_i = which_feature;
		}
	}
	// 错误率大于0.5则舍弃
	if(best_error > 0.5)
		return NULL;
	cout<<"Best feature weighted error rate "<<best_error<<endl;
	
	/** Step 3. Update the weights */
	cout << "Updating weights..." << endl;
	positive_results.clear();
	negative_results.clear();
	// 计算最佳错误率特征在正/负样本中的值
	// calcualte best_feature's value of each positive sample
	for(im_it = pos_iis.begin(); im_it != pos_iis.end(); ++im_it)
		positive_results.push_back(CalculateFeature(best_feature, *im_it));
	// calcualte best_feature's value of each negative sample
	for(im_it = neg_iis.begin(); im_it != neg_iis.end(); ++im_it)
		negative_results.push_back(CalculateFeature(best_feature, *im_it));
	// 更新正/负样本权重
	size_t i;
	double beta = (best_error)/(1 - best_error);
	cout << "Use beta = " << beta << " to reduce weight." <<endl;
	// use beta to update weights of each Positive and Negative feature
	// 如果正/负样本可以被正确分类，则更新该*样本*的权重
	assert(positive_results.size() == pos_weights.size());
	for(i = 0; i < positive_results.size(); ++i)
	{
		// Correctly identified as true, reduce weight. Else leave the same
		if(best_polarity * positive_results[i] < best_polarity * best_threshold)
			pos_weights[i] *= beta;
	}
	assert(negative_results.size() == neg_weights.size());
	for(i = 0; i < negative_results.size(); ++i)
	{
		// Correctly identified as false, reduce weight. Else leave the same
		if(best_polarity * negative_results[i] >= best_polarity * best_threshold)
			neg_weights[i] *= beta;
	}
	
	/** Return best feature */
	AdaBoostFeature* result = new AdaBoostFeature();
	result->feature = best_feature;
	result->feature_id = best_feature_i;
	result->threshold = best_threshold;
	result->polarity = best_polarity;
	result->beta_t = beta;
	return result;
}

// Yes this is inefficient. I am lazy. QED.
// Try every example value as the threshold. This is WLOG.
// 找出某个弱分类器的阈值，极性和错误率。
void FindThresholdAndPolarity(const vector<int> &positive_results, const vector<int> &negative_results, 
                              const vector<double> &pos_weights, const vector<double> &neg_weights,
                              int* threshold, int* polarity, double* error)
{
	unsigned int i, its, total_samples;
	vector<int>::const_iterator it;
	int best_threshold = 0;
	int best_polarity = 0;
	double pos_pol_sum, neg_pol_sum; // 迭代误差之和
	double best_error;
	best_error = numeric_limits<double>::infinity(); // 错误率初始化为无穷大
	// 论文中为加权错误率=w*|h-y|，当正确分类时|h-y|=0
	// for weak classifier of each sample, update threshold, polarity and error
	total_samples = positive_results.size() + negative_results.size();
	for(its = 0; its < total_samples; its++)
	{
		pos_pol_sum = 0; // 作为*正*特征的加权错误率之和 S+
		neg_pol_sum = 0; // 作为*负*特征的加权错误率之和 N+
		int cur_threshold;
		if(its < positive_results.size())
			cur_threshold = positive_results[its]; // <= as a threshold
		else
			cur_threshold = negative_results[its-positive_results.size()];
		// 计算当前*样本权值*作为*阈值*的错误率
		for(i=0; i < positive_results.size(); ++i) {
			// Misclasified with polarity of 1
			if(positive_results[i] >= cur_threshold) { 
				pos_pol_sum += pos_weights.at(i);
			}
			// Misclassified with polarity of -1
			if(positive_results[i] * -1 >= cur_threshold * -1) {
				neg_pol_sum += pos_weights.at(i);
			}
		}
		for(i=0; i < negative_results.size(); ++i) {
			// Misclasified with polarity of 1
			if(negative_results[i] < cur_threshold) { 
				pos_pol_sum += neg_weights.at(i);
			}
			// Misclassified with polarity of -1
			if(negative_results[i] * -1 < cur_threshold * -1) {
				neg_pol_sum += neg_weights.at(i);
			}
		}
		if(pos_pol_sum < best_error) {
			best_threshold = cur_threshold;
			best_polarity = 1;
			best_error = pos_pol_sum;
		}
		if(neg_pol_sum < best_error) {
			best_threshold = cur_threshold;
			best_polarity = -1; 
			best_error = neg_pol_sum;
		}
	}
	*threshold = best_threshold;
	*polarity = best_polarity;
	*error = best_error;
}

void SaveAdaBoost(vector<AdaBoostFeature*> to_save, const char* const filename)
{
	ofstream save_file;
	save_file.open(filename);
	save_file<<SUBWINDOW_SIZE<<endl;
	save_file<<to_save.size()<<endl;
	vector<AdaBoostFeature*>::const_iterator it;
	for(it = to_save.begin(); it != to_save.end(); ++it)
	{
		save_file << (*it)->feature->type << " " <<
		(*it)->feature->x1 << " " << (*it)->feature->y1 << " " <<
		(*it)->feature->x2 << " " << (*it)->feature->y2 << " " <<
		(*it)->threshold << " " << (*it)->polarity << " "<<
		(*it)->beta_t << "\n";
	}
	save_file.close();
}

