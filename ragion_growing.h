#pragma once
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stack>
#define RGB_CENTER(i) src.at<Vec3b>(center)[i]
#define RGB_ESTIMATE(i) src.at<Vec3b>(est_point)[i]
using namespace std;
using namespace cv;

class Region_Growing
{
private:
	
	Point PointShift2D[8] =
	{
				cv::Point(1, 0),
				cv::Point(1, -1),
				cv::Point(0, -1),
				cv::Point(-1, -1),
				cv::Point(-1, 0),
				cv::Point(-1, 1),
				cv::Point(0, 1),
				cv::Point(1, 1)
	};
	
	
	cv::Mat src;//picture input
	cv::Mat dest;//picture output
	cv::Mat mask;//picture output

	int threshold;
	int max_region_num;
	double min_region_num_factor;
	int min_region_area;
	Point center;
	Point est_point;
	int circle=0;
	stack<cv::Point> seed_stack;
	int delta;

	int event, x, y, flags;
	Point recent_point;
	uchar marker;
	

public:
	Region_Growing();
	Region_Growing(Mat& input);
	~Region_Growing();
	void grow(Mat& src, Mat& dest, Mat& mask, Point seed, int threshold);
	void click(int event,int x,int y,int flags,void*);
	int deal_region();

};

