#include "canny_class.h"
#include "CImg.h"
using namespace cimg_library;

int main()
{
	string file[4] = { "bigben","lena","stpietro","twows" };
	
	for (int i = 0; i < 4; i++)
	{
		canny_img tmp("test_data\\"+file[i] + ".bmp");
		//tmp.CannyDiscrete().display("i");
		tmp.CannyDiscrete();
		tmp.img_save("result\\" + file[i] + "\\");
	}

	return 0;
}