#include "GeometricTransformer.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	//srcImg: anh nguon - dstImg: anh dich
	Mat srcImg, dstImg;
	GeometricTransformer GTf;

	
	string Command(argv[1]), Interpolate(argv[2]), fileImg(argv[3]);

	srcImg = imread(fileImg, -1);

	if (Command == "--zoom") {
		float factor = atof(argv[4]);
		if (Interpolate == "--bl")
			GTf.Scale(srcImg, dstImg, factor, factor, BilinearInterpolate());
	}
	else if (Command == "--rotate") {

	}
	else if (Command == "--rotateN") {

	}
	
	waitKey(0);
	return 0;
}