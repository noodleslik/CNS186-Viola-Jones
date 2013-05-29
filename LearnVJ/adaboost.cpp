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
                 array<Mat> &pos_iis, array<Mat> &neg_iis)
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
	array<Mat> pos_iis;         // 正样本积分图
	array<Mat> neg_iis;         // 负样本积分图
	array<double> pos_weights;  // 正样本权重
	array<double> neg_weights;  // 负样本权重

	size_t num_skipped;
	num_skipped = LoadImage(which_objs, which_not_objs, pos_iis, neg_iis);
	cout << "Images are loaded. " << num_skipped << " skipped." << endl;

	// Initial weights of each POSITIVE sample  =  1/(2*n)
	for(size_t i = 0; i < pos_iis.size(); ++i)
		pos_weights.push_back((double)(1)/(double)(2 * pos_iis.size()));
	// Initial weights of each NEGATIVE sample  =  1/(2*m)
	for(size_t i = 0; i < neg_iis.size(); ++i)
		neg_weights.push_back((double)(1)/(double)(2 * neg_iis.size()));
	
	// Generate features.
	list<Feature*>* feature_list;
	if(total_set)
		feature_list = GenerateRandomFeatures(total_set);
	else
		feature_list = GenerateAllFeatures(2);
	cout << feature_list->size() << " unique features generated." << endl; 

	clock_t start;
	double minutes;
	// Run AdaBoost rounds. Get one best feature one iteration
	for(size_t i = 0; i < how_many; ++i)
	{
		start = clock();
		cout<<"====Running round "<<i<<" of Adaboost("<<feature_list->size()<<")===="<<endl;
		AdaBoostFeature* best_feature = RunAdaBoostRound(pos_iis, neg_iis, pos_weights, neg_weights, feature_list);
		if(best_feature)
		{
			afeatures.push_back(best_feature);
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

#ifdef _NUM_THREADS
#include <pthread.h>
#endif

typedef struct param
{
#ifdef _NUM_THREADS
	pthread_t thread_t;
#endif
	// input
	const array<Mat> *pos_iis, *neg_iis;
	const array<double> *pos_weights, *neg_weights;
	list<Feature*>::iterator begin, end;
	// output
	AdaBoostFeature* result;
}param;

static void* FindBestFeature_thread(void *para)
{
	param *p = (param *)para;
	int cur_threshold, best_threshold = 0;
	int cur_polarity, best_polarity = 0;
	double cur_error, best_error = numeric_limits<double>::infinity();
	double cur_false_pos_rate, best_false_pos_rate = numeric_limits<double>::infinity();
	list<Feature*>::iterator feature_it, best_feature_it;	
	// 对每个特征训练一个弱分类器，计算其加权错误率。找到错误率最小的。
	for(feature_it = p->begin; feature_it != p->end; ++feature_it)
	{
		if((*feature_it)->positive_results.empty())
			(*feature_it)->calculate_pos_results(*(p->pos_iis));
		if((*feature_it)->negative_results.empty())
			(*feature_it)->calculate_neg_results(*(p->neg_iis));
		// 找出当前弱分类器的阈值，极性和错误率。
		FindThresholdAndPolarity((*feature_it)->positive_results,
		                         (*feature_it)->negative_results,
		                         *(p->pos_weights), *(p->neg_weights),
		                         &cur_threshold, &cur_polarity,
		                         &cur_error, &cur_false_pos_rate);
		// 找到最小迭代误差之和的样本
		if(cur_error < best_error)
		{
			best_error = cur_error;
			best_threshold = cur_threshold;
			best_polarity = cur_polarity;
			best_false_pos_rate = cur_false_pos_rate;
			best_feature_it = feature_it;
		}
	}
	// 错误率大于0.5则舍弃
	if(best_error > 0.5)
	{
		p->result = NULL;
		return NULL;
	}
	p->result = new AdaBoostFeature();
	p->result->feature = *best_feature_it;
	p->result->threshold = best_threshold;
	p->result->polarity = best_polarity;
	p->result->beta_t = (best_error)/(1 - best_error);
	p->result->false_pos_rate = best_false_pos_rate;
	p->result->error_rate = best_error;
	p->result->feature_it = best_feature_it;
	return NULL;
}

AdaBoostFeature* RunAdaBoostRound(const array<Mat> &pos_iis, const array<Mat> &neg_iis,
                                  array<double> &pos_weights, array<double> &neg_weights,
                                  list<Feature*> *feature_list)
{
	AdaBoostFeature* result;
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
	result = NULL;
#ifndef _NUM_THREADS
	param para;
	para.pos_iis = &pos_iis;
	para.neg_iis = &neg_iis;
	para.pos_weights = &pos_weights;
	para.neg_weights = &neg_weights;
	para.begin = feature_list->begin();
	para.end = feature_list->end();
	FindBestFeature_thread(&para);
	result = para.result;
#else
	param para[_NUM_THREADS];
	size_t step_size = feature_list->size() / _NUM_THREADS;
	for(size_t i = 0; i < _NUM_THREADS; ++i)
	{
		para[i].pos_iis = &pos_iis;
		para[i].neg_iis = &neg_iis;
		para[i].pos_weights = &pos_weights;
		para[i].neg_weights = &neg_weights;
		para[i].begin = feature_list->begin();
		advance(para[i].begin, i * step_size);
		if(i == _NUM_THREADS - 1)
			para[i].end = feature_list->end();
		else
		{
			para[i].end = para[i].begin;
			advance(para[i].end, step_size);
		}
		printf(".");
		pthread_create(&para[i].thread_t, NULL, FindBestFeature_thread, &para[i]);
	}
	fflush(stdout);
	for(size_t i = 0; i < _NUM_THREADS; i++)
	{
		pthread_join(para[i].thread_t, NULL);
		if(result == NULL)
		{
			result = para[i].result;
		}
		else if(para[i].result != NULL)
		{
			if(para[i].result->error_rate < result->error_rate)
			{
				delete result;
				result = para[i].result;
			}
			else
				delete para[i].result;
		}
	}
	printf("merged\n");
#endif
	if(result == NULL)
		return NULL;
	cout << "Weighted error rate " << result->error_rate << endl;
	cout << "False positive rate " << result->false_pos_rate << endl;

	/** Step 3. Update the weights */
	cout << "Updating weights... ";
	cout << "beta = " << result->beta_t << endl;
	// use beta to update weights of each Positive and Negative feature
	// 如果正/负样本可以被正确分类，则更新该*样本*的权重
	const array<int> &positive_results = result->feature->positive_results;
	const array<int> &negative_results = result->feature->negative_results;
	assert(positive_results.size() == pos_weights.size());
	for(size_t i = 0; i < positive_results.size(); ++i)
	{
		// Correctly identified as true, reduce weight. Else leave the same
		if(result->polarity * positive_results[i] < result->polarity * result->threshold)
			pos_weights[i] *= result->beta_t;
	}
	assert(negative_results.size() == neg_weights.size());
	for(size_t i = 0; i < negative_results.size(); ++i)
	{
		// Correctly identified as false, reduce weight. Else leave the same
		if(result->polarity * negative_results[i] >= result->polarity * result->threshold)
			neg_weights[i] *= result->beta_t;
	}

	/** Remove best feature from feature list */
	feature_list->erase(result->feature_it);

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

