#include "ragion_growing.h"

using namespace std;
using namespace cv;

Region_Growing::Region_Growing(void)
{
	
}

Region_Growing::Region_Growing(Mat& input)
{
	src = input;
	if (src.cols > 500 || src.rows > 500) {
		cv::resize(src, src, cv::Size(0, 0), 0.5, 0.5); // resize for speed
	}
	//imshow("input", src);


	threshold = 200;

	//src = imread("F:\\bird.jpg");


	min_region_area = static_cast<int>(min_region_num_factor * src.rows * src.cols);
	marker = 1;
	delta = 0;
	event = 0;
	x = 0;
	y = 0;
	flags = 0;
}



Region_Growing::~Region_Growing()
{
}

void Region_Growing::grow(Mat& src, Mat& dest, Mat& mask, Point seed, int threshold)
{
	seed_stack.push(seed);
	imshow("input", src);
	while (!seed_stack.empty())
	{
		center = seed_stack.top();
		mask.at<uchar>(center) = 1;
		seed_stack.pop();
		for (int i = 0; i < 8; i++)
		{
			est_point = center + PointShift2D[i];
			if (est_point.x<0 || est_point.y<0 || est_point.x>src.cols - 1 || est_point.y>src.rows - 1)
			{
				continue;
			}
			else
			{
				delta = int(pow(RGB_CENTER(0) - RGB_ESTIMATE(0), 2) +\
					        pow(RGB_CENTER(1) - RGB_ESTIMATE(1), 2) +\
					        pow(RGB_CENTER(2) - RGB_ESTIMATE(2), 2));
				if (dest.at<uchar>(est_point) == 0 && mask.at<uchar>(est_point) == 0 && delta < threshold)
				{
					mask.at<uchar>(est_point) = 1;
					seed_stack.push(est_point);
				}
			}
		}
	}
}

void Region_Growing::click(int event, int x, int y, int flags, void*)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		recent_point = cv::Point(x, y);
		cout << recent_point.x << " " << recent_point.y << " " << endl;
	}
}

int Region_Growing::deal_region()
{
	Mat dest = Mat::zeros(src.rows, src.cols, CV_8UC1);
	Mat mask = Mat::zeros(src.rows, src.cols, CV_8UC1);
	for (int i = 0; i < src.cols; ++i)
	{
		for (int j = 0; j < src.rows; ++j)
		{
			if (dest.at<uchar>(cv::Point(i, j)) == 0) {
				grow(src, dest, mask, cv::Point(i, j), threshold);
				int mask_area = (int)cv::sum(mask).val[0];  // calculate area of the region that we get in "seed grow"
				if (mask_area > min_region_area) {
					dest = dest + mask * marker;   // record new region to "dest"
					cv::imshow("mask", mask * 255);//turn marker to white
					cv::imshow("dest", dest * 255);//same
					//cout << mask;
					//cout << i << j << endl;
					cv::waitKey();
					if (++marker > max_region_num) { printf("run out of max_region_num..."); return -1; }
				}
				else {
					dest = dest + mask * 255;   // record as "ignored"
				}
				mask = mask - mask;     // zero mask, ready for next "seed grow"
			}
		}
	}
}

int main()
{
	Mat input = imread("F:\\bird.jpg");
	//Region_Growing picture();
	Region_Growing picture(input);
	//Mat src = imread("F:\\bird.jpg");
	picture.deal_region();
	
	
}
