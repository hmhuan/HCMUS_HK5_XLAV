#include "SVD.h"

//int main(int argc, char * argv[])
//{
//	Mat srcImage, cvtImage, dstImage, w, u, vt;
//	//string Command(argv[1]), InputPath(argv[2]), OutputPath(argv[3]);
//	int result = 0, type;
//
//	//srcImage = imread(InputPath, -1);
//	//if (!srcImage.empty())
//	//	imshow("Original Image", srcImage);
//	//if (Command == "--compress")
//	//{
//	//	int k = atoi(argv[4]);
//	//	type = 1;
//	//	result = Compress(srcImage, dstImage, k);
//	//}
//	//else if (Command == "--decompress")
//	//{
//	//	type = 2;
//	//	result = Decompress(srcImage, dstImage);
//	//}
//
//	//if (result)
//	//{
//	//	//Nếu decompress hiển thị ảnh giải nén
//	//	if (type == 2)
//	//		imshow("Decompress Image", dstImage);
//	//	imwrite(OutputPath, dstImage);
//	//}
//	//waitKey(0);
//
//	return 0;
//}

int main()
{
	// Image matrix
	Mat img, cvtImage;
	Mat result;
	
	img = imread("traffic.png", -1);
	cout << img.type() << endl;
	if (img.type() != CV_8UC1)
		cvtColor(img, cvtImage, CV_BGR2GRAY);
	else
		cvtImage = img.clone();
	cvtImage.convertTo(cvtImage, CV_32FC1, 1/255.0f);
	cout << "Source size:" << img.rows*img.cols << " elements " << endl;
	// create SVD 
	cv::SVD s;
	// svd result
	Mat w, u, vt;
	// computations ...
	s.compute(cvtImage, w, u, vt, SVD::FULL_UV);
	//imwrite("w.png", w);
	//imwrite("u.png", u);
	//imwrite("vt.png", vt);
	cout << u.rows << " " << u.cols << endl;
	cout << w.rows << " " << w.cols << endl;
	cout << vt.rows << " " << vt.cols << endl;
	cout << w << endl;
	// collect Sigma matrix (diagonal - is eigen values, other - zeros)
	// we got it in as vector, transform it to diagonal matrix
	Mat W = Mat::diag(w);
	/*for (int i = 0; i<w.rows; i++)
	{
		W.at<float>(i, i) = w.at<float>(i);
	}*/

	// reduce rank to k
	int k = 100;
	W = W(Range(0, k), Range(0, k));
	u = u(Range::all(), Range(0, k));
	vt = vt(Range(0, k), Range::all());

	// Get compressed image
	result = u*W*vt;
	//cout << "Result size:" << u.rows*u.cols + k + vt.rows*vt.cols << " elements " << endl;
	//---------------------------------------------
	//
	//--------------------------------------------- 
	//result.convertTo(result, CV_8UC1, 255.0f);
	imshow("Source Image", img);
	imshow("Result", result);
	//vector<int> compression_params;
	//compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	//compression_params.push_back(0);
	//imwrite("compress.png", result);
	cvWaitKey(0);
	return 0;
}