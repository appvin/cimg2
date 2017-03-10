#pragma once

#include "CImg.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace cimg_library;
using namespace std;

class canny_img
{
private:
	CImg<float> img;//读入图像
	CImg<float> in;//灰度处理后的图像
	CImg<float> outSmooth;//平滑后的图像
	CImg<float> outGradient;//图像梯度
	CImg<float> outOrientation;
	CImg<float> outNMS;//non-max-supression
	CImg<float> outThreshold;//阈值处理后图像
	int widthIn, heightIn;
	float sigma;
	float threshold;
	CImg<int> dirmax;
	CImg<float> derivative[4];

public:
	canny_img(string infile, float sigma = 1.5f, float threshold = 3.0f);
	void init_memory();
	CImg<float> gray();//灰度处理
	CImg<float> CannyDiscrete();//canny算法
	CImg<float> gauss_filter(CImg<float>& filter, int deriv = 0);//高斯滤波
	CImg<float> gradient();//梯度计算
	CImg<float> thres_nms();//阈值与non-max-supression处理
	void img_save(string path);
};