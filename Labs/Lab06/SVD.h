#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <fstream>
#include <stdio.h>
#include <vector>
using namespace std;
using namespace cv;

//bool writeMatBinary(FILE * & ofs, const Mat & out_mat)
//{
//	int sizeImg[3] = { out_mat.rows, out_mat.cols, out_mat.type() };
//	fwrite(sizeImg, 3, sizeof(int), ofs);
//	for (int i = 0; i < out_mat.rows; i++)
//		for (int j = 0; j < out_mat.cols; j++)
//		{
//			fwrite(&out_mat.at<float>(i, j), sizeof(float), 1, ofs);
//		}
//	return 1;
//}

// Hàm ghi ma trận ra file
// Các tham số: file ghi ra, ma trận cần ghi ra file
// Output: thành công/ thất bài (true/ false)
bool writeMatBinary(fstream& ofs, const Mat & out_mat)
{
	// Kiểm tra file có mở chưa/ tồn tại không
	if (!ofs.is_open())
	{
		return false;
	}
	// kiểm out_mat rỗng
	if (out_mat.empty()) {
		int s = 0;
		ofs.write((const char*)(&s), sizeof(int));
		return true;
	}
	int type = out_mat.type();
	ofs.write((const char*)(&out_mat.rows), sizeof(int));
	ofs.write((const char*)(&out_mat.cols), sizeof(int));
	ofs.write((const char*)(&type), sizeof(int));
	//ofs.write((const char*)(out_mat.data), out_mat.rows * out_mat.cols * out_mat.elemSize());
	for (int i = 0; i < out_mat.rows; i++)
		for (int j = 0; j < out_mat.cols; j++)
		{
			ofs.write((const char *)&out_mat.at<float>(i, j), sizeof(float));
		}
	return true;
}

// Hàm đọc ma trận ảnh từ file
// Các tham số: file đầu vào, ảnh đọc từ file
// Output: thành công hay không (true/false)
bool readMatBinary(fstream& ifs, Mat& in_mat)
{
	if (!ifs.is_open()) {
		return false;
	}

	int rows, cols, type;
	float val;
	ifs.read((char*)(&rows), sizeof(int));
	ifs.read((char*)(&cols), sizeof(int));
	ifs.read((char*)(&type), sizeof(int));

	in_mat.release();
	in_mat.create(rows, cols, type);
	//ifs.read((char*)(in_mat.data), in_mat.rows * in_mat.cols * sizeof(float));
	for (int i=0; i< rows; i++)
		for (int j = 0; j < cols; j++)
		{
			ifs.read((char *)&val, sizeof(val));
			in_mat.at<float>(i, j) = val;
		}
	return true;
}

// Hàm nhân 2 ma trận trả về ma trận kết quả
// Các tham số: 2 ma trận đầu vào
// Output: ma trận kết quả sau khi thực hiện nhân
Mat Multiply(Mat & A, Mat & B)
{
	Mat result;
	if (A.cols != B.rows)
		return result;
	// Khởi tạo ma trận kết quả có A-dòng, B-cột
	result = Mat::zeros(A.rows, B.cols, A.type());
	int height = result.rows, width = result.cols, kSize = A.cols;
	//int widthStepA = A.rows , widthStepB = B.rows, widthStep = result.rows;
	float sum;
	// Con trỏ 3 ma trận ảnh
	/*float * pA, *pB, *pData, *pAr, *pBr, *pRow;
	pA = (float *)A.data;
	pB = (float *)B.data;
	pData = (float *)result.data;*/

	/*for (int i = 0; i < height; i++, pA += widthStepA, pData += widthStep)
	{
		pRow = pData; pAr = pA; pBr = pB;
		for (int j = 0; j < width; j++, pRow += 1, pBr += 1)
		{
			
			sum = 0.0;
			for (int k = 0; k < kSize; k++, pAr += 1, pBr += widthStepB)
			{
				sum += pAr[0] * pBr[0];
			}
			pRow[0] = sum;
		}
	}*/
	for (int i=0;i<height; i++)
		for (int j = 0; j < width; j++)
		{
			sum = 0.0;
			for (int k = 0; k < kSize; k++)
				sum += A.at<float>(i, k) * B.at<float>(k, j);
			result.at<float>(i, j) = sum;
		}
	return result;
}

// Hàm nén ảnh
// Các tham số: 1: ảnh nguồn
//				2, 3, 4: các file để lưu 3 ma trận U, W, Vt
//				5: Số chiều cần giảm
int Compress(const Mat& srcImage, fstream & fU, fstream &fW, fstream  &fVt, int k)
{
	// Kiểm tra ảnh nguồn
	if (srcImage.empty())
		return 0;
	bool check;
	Mat cvtImage, w, u, vt, result;
	SVD s;
	// chuyển sang ảnh xám và convert định dạng ảnh về type CV_32FC1
	if (srcImage.type() != CV_8UC1)
		cvtColor(srcImage, cvtImage, CV_BGR2GRAY);
	else
		cvtImage = srcImage.clone();
	cvtImage.convertTo(cvtImage, CV_32FC1, 1.0/255.0);
	cout << cvtImage.type() << endl;

	//Tạo svd và tính 3 ma trận U, W, V
	// computations ...
	cout << "SVD Computating ...\n";
	s.compute(cvtImage, w, u, vt, SVD::FULL_UV);
	cout << "Done.\n";
	
	// Ma trận W là ma trận vuông có các giá trị trên đường chéo chính là w
	Mat W = Mat::diag(w);

	// Giảm số chiều 3 ma trận
	if (k > W.rows)
		k = W.rows;
	u = u(Range::all(), Range(0, k));
	W = W(Range(0, k), Range(0, k));
	vt = vt(Range(0, k), Range::all());

	cout <<"Size of U: "<< u.rows << " " << u.cols << endl;
	cout << "Size of W: "<< W.rows << " " << W.cols << endl;
	cout <<"Size of Vt: "<< vt.rows << " " << vt.cols << endl;

	//result = Multiply(Multiply(u, W), vt);//result = u*W*vt;
	//imshow("Compressed", result);
	
	// Ghi 3 ma trận ra file
	cout << "Write file ...\n";
	check = writeMatBinary(fU, u);
	check = writeMatBinary(fW, W);
	check = writeMatBinary(fVt, vt);
	cout << "Done.\n";
	/*vector<int> params;
	params.push_back(IMWRITE_PXM_BINARY);
	params.push_back(1);
	imwrite("u.pgm", u, params);
	imwrite("W.pgm", W, params);
	imwrite("vt.pgm", vt, params);*/
	return 1;
}

// Hàm giải nén từ 3 ma trận U, W, Vt
// Các tham số: 1, 2, 3: 3 file chứa 3 ma trận U, W, Vt
//				4: Ảnh đích sau khi được giải nén
int Decompress(fstream & fU, fstream & fW, fstream & fVt, Mat & dstImage)
{
	// Xử lý
	Mat u, W, vt, result;
	bool check;
	check = readMatBinary(fU, u);
	check = readMatBinary(fW, W);
	check = readMatBinary(fVt, vt);
	if (!check)
		return 0;

	/*u = imread("u.pgm", -1);
	W = imread("W.pgm", -1);
	vt = imread("vt.pgm", -1);
	u.convertTo(u, CV_32FC1);
	W.convertTo(W, CV_32FC1);
	vt.convertTo(vt, CV_32FC1);*/

	cout << u.rows << " " << u.cols << " " << u.type() << endl;
	cout << W.rows << " " << W.cols << " " << W.type() << endl;
	cout << vt.rows << " " << vt.cols << " " << vt.type() << endl;
	cout << "Decompressing ...\n";
	dstImage = Multiply(Multiply(u, W), vt);
	//dstImage = u * W * vt;
	dstImage.convertTo(dstImage, CV_8UC1, 255.0f);
	cout << "Done.\n";
	if (dstImage.empty())
		return 0;
	return 1;
}