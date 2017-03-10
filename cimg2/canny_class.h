#pragma once

#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

class canny_img
{
private:
	CImg<float> img;//����ͼ��
	CImg<float> in;//�Ҷȴ�����ͼ��
	CImg<float> outSmooth;//ƽ�����ͼ��
	CImg<float> outGradient;//ͼ���ݶ�
	CImg<float> outOrientation;
	CImg<float> outNMS;//non-max-supression
	CImg<float> outThreshold;//��ֵ�����ͼ��
	int widthIn, heightIn;
	float sigma = 1.5f;
	float threshold = 7.0f;
	CImg<int> dirmax;
	CImg<float> derivative[4];

public:
	canny_img(string infile);
	void init_memory();
	CImg<float> to_gray();//�Ҷȴ���
	CImg<float> CannyDiscrete();//canny�㷨
	CImg<float> gauss_filter(CImg<float>& filter, int deriv = 0);//��˹�˲�
	CImg<float> gradient();//�ݶȼ���
	CImg<float> thres_nms();//��ֵ��non-max-supression����
};