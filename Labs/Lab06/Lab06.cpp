#include "SVD.h"

int main(int argc, char * argv[])
{
	Mat srcImage, cvtImage, dstImage, w, u, vt;
	string Command(argv[1]), InputPath(argv[2]), OutputPath(argv[3]);
	int result = 0, type;

	//Read Image with type gray 
	//srcImage = imread("2.png", CV_8UC1);
	////Convert Image type to CV_32F
	//srcImage.convertTo(cvtImage, CV_32F);
	//int height = srcImage.rows, width = srcImage.cols;
	//cout << srcImage.type() << endl;
	//SVD svd(srcImage);	
	//cout << height << " " << width << "\n\n";

	//cout << svd.u.rows << " " << svd.u.cols << endl;
	//cout << svd.vt.rows << " " << svd.vt.cols << endl;
	////cout << svd.w << "\n";

	srcImage = imread(InputPath, -1);
	if (!srcImage.empty())
		imshow("Original Image", srcImage);
	if (Command == "--compress")
	{
		int k = atoi(argv[4]);
		type = 1;
		result = Compress(srcImage, dstImage, k);
	}
	else if (Command == "--decompress")
	{
		type = 2;
		result = Decompress(srcImage, dstImage);
	}

	if (result)
	{
		//Nếu decompress hiển thị ảnh giải nén
		if (type == 2)
			imshow("Decompress Image", dstImage);
		imwrite(OutputPath, dstImage);
	}
	waitKey(0);
	return 0;
}