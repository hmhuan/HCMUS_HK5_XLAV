#pragma once
#include "Threshold.h"

class Kmean
{
	//số cụm K
	int _numClusters;
public:
	/*
	Hàm áp dụng thuật toán Kmeans để phân đoạn ảnh
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	Hàm trả về
	1: nếu phân đoạn thành công
	0: nếu phân đoạn không thành công
	*/
	void setNumClusters(int num)
	{
		_numClusters = num;
	}
	int Apply(const Mat& srcImage, Mat &dstImage)
	{
		return 0;
	}

	Kmean() { _numClusters = 2; }
	~Kmean() {}
};

