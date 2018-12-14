#pragma once
#include "Threshold.h"

struct Histogram
{
	int count, id;
	void operator=(Histogram & his)
	{
		int temp;
		temp = count; 
		count = his.count;
		his.count = temp;
		temp = id;
		id = his.id;
		his.id = temp;
	}
};

//void Sort(Histogram his[])
//{
//	Histogram temp;
//	for(int i=0;i<255;i++)
//		for(int j=i+1;j<256;j++)
//			if (his[i].count < his[j].count)
//			{
//				temp = his[i];
//				his[i] = his[j];
//				his[j] = temp;
//			}
//}

void InitializeKClusters(const Mat & srcImage, Histogram his[], int kClusters, vector<Scalar> & clustersCenters, vector< vector<Point> > &ElementsOfKthCluster)
{
	// Khởi tạo lớp random của OpenCV hỗ trợ
	RNG random(cv::getTickCount());

	for (int k = 0; k < kClusters; k++) 
	{
		// get random pixel in image to initialize cluster center
		// Lấy ngẫu nhiên các điểm ảnh để khởi tạo centroid cluster
		Point centerKPoint;
		centerKPoint.x = random.uniform(0, srcImage.rows);
		centerKPoint.y = random.uniform(0, srcImage.cols);
		Scalar centerPixel = srcImage.at<uchar>(centerKPoint.x, centerKPoint.y);
		// get color value of pixel and save it as a center
		// Lấy giá trị màu của điểm ảnh
		Scalar centerK(centerPixel.val[0]);
		clustersCenters.push_back(centerK);
		//T
		vector<Point> ClusterK;
		ClusterK.push_back(centerKPoint);
		ElementsOfKthCluster.push_back(ClusterK);
	}
}

//Tính Distance theo màu
double computeColorDistance(Scalar pixel, Scalar clusterPixel) 
{
	// Sử dụng độ đo euclid để tính color distance
	double delta = pixel.val[0] - clusterPixel.val[0];
	double distance = sqrt(delta * delta);
	return distance;
}

double computeError(double pixel, double mean)
{
	double delta = pixel - mean;
	return delta * delta;
}
// Chia các điểm ảnh về các cluster tương ứng
void UpdateClusters(const Mat &srcImg, int clusters_number, vector<Scalar> & clustersCenters, vector<vector<Point>> & ElementsOfKthCluster) 
{
	double * W, value = 0;
	int height = srcImg.rows, width = srcImg.cols;

	W = new double[clusters_number];
	for (int k = 0; k < clusters_number; k++)
		W[k] = 0.0;
	// For each pixel, find closest cluster
	for (int r = 0; r<height; r++) 
	{
		for (int c = 0; c<width; c++) 
		{

			double minDistance = INFINITY; // khoảng cách màu
			int id = 0; // Lưu lại vị trí cluster mà điểm ảnh gần nó nhất

			Scalar pixel = srcImg.at<uchar>(r, c); // Lấy ra điểm ảnh taiij vị trí (r, c)
			value = pixel.val[0];
			for (int k = 0; k < clusters_number; k++) 
			{

				Scalar centroid = clustersCenters[k];

				// Tính khoảng cách màu của điểm ảnh với centroid
				double distance = computeColorDistance(pixel, centroid);
				// Cập nhật
				if (distance < minDistance) 
				{
					minDistance = distance;
					id = k;
				}
			}
			// Cập nhật W[id] và Lưu điểm ảnh vào cluster id tương ứng
			W[id] += value;
			ElementsOfKthCluster[id].push_back(Point(r, c));
		}
	}
	//Cập nhật lại giá trị centroid của cluster
	for (int k = 0; k < clusters_number; k++)
	{
		W[k] /= ElementsOfKthCluster[k].size();
		clustersCenters[k].val[0] = W[k];
	}
}
// Cập nhật lại center của các Clusters
// Trả về giá trị là độ chênh lệch giữa 2 giá trị lỗi trước và hiện hành
double ErrorFunction(const Mat & srcImg, int clusters_number, vector<Scalar> & clustersCenters, vector<vector<Point>> & ElementsOfKthCluster, double & oldError) {

	double diffChange;
	double newError = 0;
	
	// Update các centroid của các clusters
	//for (int k = 0; k < clusters_number; k++) 
	//{
	//	vector<Point> KthCluster = ElementsOfKthCluster[k];
	//	double newMean = 0;
	//	// Tính lại giá trị mean của cluster thứ k
	//	for (int i = 0; i<KthCluster.size(); i++) 
	//		newMean += srcImg.at<uchar>(KthCluster[i].x, KthCluster[i].y);
	//	newMean /= KthCluster.size();
	//	Scalar newPixel(newMean);
	//	clustersCenters[k] = newPixel;
	//}

	// Tính lại Error
	for (int k = 0; k < clusters_number; k++)
	{
		// Error = Sigma (k = 0 -> nclusters) sigma (các phần tử - mean) ^ 2;
		vector<Point> KthCluster = ElementsOfKthCluster[k];
		double mean = clustersCenters[k].val[0];
		for (int i = 0; i < KthCluster.size(); i++)
			newError += computeError(srcImg.at<uchar>(KthCluster[i].x, KthCluster[i].y), mean);
	}
	// Tính độ chênh lệch giữa giá trị lỗi
	diffChange = abs(oldError - newError);
	//cout << "Error change is: " << (int)diffChange << endl;
	printf("Error change is: %0.4f\n", diffChange);
	oldError = newError;

	return diffChange;
}

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
		Mat Gray;
		// Danh sách các giá trị centroid của các clusters
		vector<Scalar> clustersCenters;
		// Danh sách chứa các điểm thuộc Cluster thứ k
		vector< vector<Point> > ElementsOfKthCluster;
		Histogram his[256];
		double epsilon = 0.05;
		double oldError = INFINITY;
		double newError = 0;
		double diffChange = oldError - newError;
		//Khởi tạo ảnh đích và ảnh xám của ảnh nguồn
		if (srcImage.empty())
			return 0;
		if (srcImage.type() != CV_8UC1)
			cvtColor(srcImage, Gray, CV_BGR2GRAY);
		else
			Gray = srcImage.clone();
		imshow("Gray", Gray);
		dstImage.create(Gray.rows, Gray.cols, Gray.type());
		
		// Khởi tạo các centroids thuộc các Clusters
		InitializeKClusters(Gray, his, _numClusters, clustersCenters, ElementsOfKthCluster);

		// Lặp đến khi mức độ thay đổi nhỏ hơn epsilon
		while (diffChange > epsilon) 
		{
			for (int k = 0; k<_numClusters; k++) 
				ElementsOfKthCluster[k].clear();

			// Cập nhật cluster và centroid
			UpdateClusters(Gray, _numClusters, clustersCenters, ElementsOfKthCluster);

			// Tính độ lệch hàm lỗi
			diffChange = ErrorFunction(srcImage, _numClusters, clustersCenters, ElementsOfKthCluster, oldError);
		}
		// Lưu ra ảnh đích
		for (int k = 0; k < _numClusters; k++)
		{
			vector<Point> ClusterKth = ElementsOfKthCluster[k];
			int bin = 255 / _numClusters * (k + 1);
			//for each pixel in cluster change color to fit cluster
			for (int i = 0; i < ClusterKth.size(); i++)
				dstImage.at<uchar>(ClusterKth[i].x, ClusterKth[i].y) = clustersCenters[k].val[0];
		}
		Gray.release();
		if (dstImage.empty())
			return 0;
		return 1;
	}

	Kmean() { _numClusters = 2; }
	~Kmean() {}
};

