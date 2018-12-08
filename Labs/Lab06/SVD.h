#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"

using namespace std;
using namespace cv;

int Compress(const Mat& srcImage, Mat &dstImage, int k)
{
	if (srcImage.empty())
		return 0;
	Mat cvtImage;
	cvtColor(srcImage, cvtImage, CV_BGR2GRAY);
	imshow("Gray", cvtImage);
	cvtImage.convertTo(cvtImage, CV_32FC1);
	cout << cvtImage.type() << endl;
	//Tạo svd
	SVD svd(cvtImage, SVD::FULL_UV);
	cout << "SVD done!!!" << endl;
	cout << svd.u.rows << " " << svd.u.cols << endl;
	cout << svd.w.rows << " " << svd.w.cols << endl;
	cout << svd.vt.rows << " " << svd.vt.cols << endl;
	//Thực hiện nén 
	if (dstImage.empty())
		return 0;
	return 1;
}

int Decompress(const Mat& srcImage, Mat &dstImage)
{
	if (srcImage.empty())
		return 0;




	if (dstImage.empty())
		return 0;
	return 1;
}