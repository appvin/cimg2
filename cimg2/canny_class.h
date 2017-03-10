#pragma once

#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

class canny_img
{
private:
	CImg<float> img;
	CImg<float> in;
	CImg<float> outSmooth;
	CImg<float> outGradient;
	CImg<float> outOrientation;
	CImg<float> outNMS;
	CImg<float> outThreshold;
	int widthIn, heightIn;
	float sigma = 1.5f;
	float threshold = 7.0f;
	CImg<int> dirmax;
	CImg<float> derivative[4];

public:
	canny_img(string infile);
	void init_memory();
	CImg<float> to_gray();
	CImg<float> CannyDiscrete();
	CImg<float> gauss_filter(CImg<float>& filter, int deriv = 0);
	CImg<float> gradient();
	CImg<float> thres_nms();
};