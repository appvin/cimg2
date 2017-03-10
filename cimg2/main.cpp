#include <iostream>
#include "canny_class.h"
using namespace std;

int main()
{
	string infile = "input.bmp";
	canny_img cny(infile);
	cny.CannyDiscrete().display("gray");
	
	return 0;
}