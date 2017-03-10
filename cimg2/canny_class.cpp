#include "canny_class.h"
#include <string>

using namespace std;
using namespace cimg_library;

canny_img::canny_img(string infile, float sigma, float threshold)
{
	img=CImg<float>(infile.c_str());
	widthIn = img._width;
	heightIn = img._height;
	if (widthIn == 0 || heightIn == 0) {
		cerr << "Error when loading input image." << endl;
		return;
	}
	this->sigma = sigma;
	this->threshold = threshold;
	CImg<int> dirmax(outGradient);
}

void canny_img::init_memory()
{
	outGradient = in; outGradient.fill(0.0f);
	dirmax = CImg<int>(outGradient);
	for (int i = 0; i < 4; i++) { derivative[i] = outGradient; }
	outOrientation = outGradient;
	outThreshold = outGradient;
	outNMS = outGradient;
}

CImg<float> canny_img::gray()
{
	in = CImg<float>(widthIn, heightIn, 1, 1, 0);
	cimg_forXY(img, x, y)
	{
		in(x, y) = float(img(x, y, 0)*0.299 + img(x, y, 1)*0.587 + img(x, y, 2)*0.114);
	}
	return in;
}

CImg<float> canny_img::gauss_filter(CImg<float>& filter, int deriv)
{
	float width = 3 * sigma;
	float sigma2 = sigma*sigma;
	filter.assign(int(2 * width) + 1);
	int i = 0;
	for (float x = -width; x <= width; x += 1.0f) {
		float g = float(exp(-0.5*x*x / sigma2) / sqrt(2 * cimg::PI) / sigma);
		if (deriv == 1) g *= -x / sigma2;
		if (deriv == 2) g *= (x*x / sigma2 - 1.0f) / sigma2;
		filter[i] = g;
		i++;
	}
	return filter;
}

CImg<float> canny_img::gradient()
{
	/* loop over all pixels in the interior */
	float fct = float(1.0 / (2.0*sqrt(2.0f)));
	for (int y = 1; y < heightIn - 1; y++) {
		for (int x = 1; x < widthIn - 1; x++) {
			/* compute directional derivatives (E,NE,N,SE) */
			float grad_E = float((outSmooth(x + 1, y) - outSmooth(x - 1, y))*0.5);
			float grad_NE = (outSmooth(x + 1, y - 1) - outSmooth(x - 1, y + 1))*fct;
			float grad_N = float((outSmooth(x, y - 1) - outSmooth(x, y + 1))*0.5);
			float grad_SE = (outSmooth(x + 1, y + 1) - outSmooth(x - 1, y - 1))*fct;
			/* compute gradient magnitude */
			float grad_mag = grad_E*grad_E + grad_N*grad_N;
			outGradient(x, y) = grad_mag;

			/* compute gradient orientation (continuous version)*/
			float angle = 0.0f;
			if (grad_mag > 0.0f) { angle = atan2(grad_N, grad_E); }
			if (angle < 0.0) angle += float(cimg::PI);
			outOrientation(x, y) = float(angle*255.0 / cimg::PI + 0.5);

			/* compute absolute derivations */
			derivative[0](x, y) = grad_E = fabs(grad_E);
			derivative[1](x, y) = grad_NE = fabs(grad_NE);
			derivative[2](x, y) = grad_N = fabs(grad_N);
			derivative[3](x, y) = grad_SE = fabs(grad_SE);

			/* compute direction of max derivative */
			if ((grad_E>grad_NE) && (grad_E>grad_N) && (grad_E>grad_SE)) {
				dirmax(x, y) = 0;
			}
			else if ((grad_NE>grad_N) && (grad_NE>grad_SE)) {
				dirmax(x, y) = 1;
			}
			else if (grad_N>grad_SE) {
				dirmax(x, y) = 2;
			}
			else {
				dirmax(x, y) = 3;
			}
		}
	}
	return outGradient;
}

CImg<float> canny_img::thres_nms()
{
	// directing vectors (E, NE, N, SE)
	int dir_vector[4][2] = { { 1,0 },{ 1,-1 },{ 0,-1 },{ 1,1 } };
	// direction of max derivative of
	// current pixel and its two neighbouring pixel (in direction of dir)
	int dir, dir1, dir2;

	/* thresholding and (canny) non-max-supression */
	for (int y = 2; y < heightIn - 2; y++) {
		for (int x = 2; x < widthIn - 2; x++) {
			dir = dirmax(x, y);
			if (derivative[dir](x, y) < threshold) {
				outThreshold(x, y) = 0.0f;
				outNMS(x, y) = 0.0f;
			}
			else {
				outThreshold(x, y) = 255.0f;
				int dx = dir_vector[dir][0];
				int dy = dir_vector[dir][1];
				dir1 = dirmax(x + dx, y + dy);
				dir2 = dirmax(x - dx, y - dy);
				outNMS(x, y) = 255.f*
					((derivative[dir](x, y) > derivative[dir1](x + dx, y + dy)) &&
					(derivative[dir](x, y) >= derivative[dir2](x - dx, y - dy)));
			} // -> outThreshold, outNMS
		}
	} // for x, y...
	return outThreshold;
}

void canny_img::img_save(string path)
{
	img.save((path + "1.source.bmp").c_str());
	in.save((path + "2.gray.bmp").c_str());
	outSmooth.save((path + "3.smooth.bmp").c_str());
	outGradient.save((path + "4.gradient.bmp").c_str());
	outOrientation.save((path + "5.orientation.bmp").c_str());
	outNMS.save((path + "6.non-max-supression.bmp").c_str());
	outThreshold.save((path + "7.threshold.bmp").c_str());
	outNMS.save((path + "8.result.bmp").c_str());
}

CImg<float> canny_img::CannyDiscrete()
{
	gray();

	init_memory();
	
	/* smoothing the input image */
	CImg<float> filter;
	gauss_filter(filter);
	outSmooth = in.get_convolve(filter).convolve(filter.get_transpose());

	gradient();

	thres_nms();

	return outNMS;
}