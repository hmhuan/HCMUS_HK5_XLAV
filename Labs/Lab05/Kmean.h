#pragma once
#include "Threshold.h"

void InitializeKClusters(const Mat & srcImage, int kClusters, vector<Scalar> & clustersCenters, vector< vector<Point> > &ElementsOfKthCluster)
{

	RNG random(cv::getTickCount());

	for (int k = 0; k<kClusters; k++) 
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
	//use euclidian distance to get distance
	double distance = abs(pixel.val[0] - clusterPixel.val[0]);
	return distance;
}

void findAssociatedCluster(const Mat &srcImg, int clusters_number, vector<Scalar> & clustersCenters, vector<vector<Point>> & ElementsOfKthCluster) 
{

	int height = srcImg.rows, width = srcImg.cols;
	// For each pixel, find closest cluster
	for (int r = 0; r<height; r++) 
	{
		for (int c = 0; c<width; c++) 
		{

			double minDistance = INFINITY;
			int id = 0;
			Scalar pixel = srcImg.at<uchar>(r, c);

			for (int k = 0; k<clusters_number; k++) {

				Scalar centroid = clustersCenters[k];

				//use color difference to get distance to cluster
				double distance = computeColorDistance(pixel, centroid);

				//update to closest cluster center
				if (distance < minDistance) {
					minDistance = distance;
					id = k;
				}
			}
			//save pixel into associated cluster
			ElementsOfKthCluster[id].push_back(Point(r, c));
		}
	}
}

//Cập nhật lại center của các clusters
double updateClusterCenters(const Mat & srcImg, int clusters_number, vector<Scalar> & clustersCenters, vector<vector<Point>> & ElementsOfKthCluster, double & oldCenter, double newCenter) {

	double diffChange;

	//adjust cluster center to mean of associated pixels
	for (int k = 0; k<clusters_number; k++) {

		vector<Point> KthCluster = ElementsOfKthCluster[k];
		double newMean = 0;

		//compute mean values for 3 channels
		for (int i = 0; i<KthCluster.size(); i++) {
			Scalar pixel = srcImg.at<uchar>(KthCluster[i].x, KthCluster[i].y);
			newMean += pixel.val[0];
		}

		newMean /= KthCluster.size();

		//assign new color value to cluster center
		Scalar newPixel(newMean);

		//compute distance between the old and new values
		newCenter += computeColorDistance(newPixel, clustersCenters[k]);
		clustersCenters[k] = newPixel;

	}

	newCenter /= clusters_number;

	//get difference between previous iteration change
	diffChange = abs(oldCenter - newCenter);
	cout << "diffChange is: " << diffChange << endl;
	oldCenter = newCenter;

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
		// Danh sách 
		vector< vector<Point> > ElementsOfKthCluster;
		//Khởi tạo ảnh đích và ảnh xám của ảnh nguồn

		double epsilon = 0.05;
		double oldCenter = INFINITY;
		double newCenter = 0;
		double diffChange = oldCenter - newCenter;

		if (srcImage.empty())
			return 0;
		if (srcImage.type() != CV_8UC1)
			cvtColor(srcImage, Gray, CV_BGR2GRAY);
		else
			Gray = srcImage.clone();
		dstImage.create(Gray.rows, Gray.cols, Gray.type());
		
		InitializeKClusters(Gray, _numClusters, clustersCenters, ElementsOfKthCluster);

		//iterate until cluster centers nearly stop moving (using threshold)
		while (diffChange > epsilon) 
		{
			//reset change
			newCenter = 0;

			//clear associated pixels for each cluster
			for (int k = 0; k<_numClusters; k++) 
				ElementsOfKthCluster[k].clear();

			//find all closest pixel to cluster centers
			findAssociatedCluster(Gray, _numClusters, clustersCenters, ElementsOfKthCluster);

			//recompute cluster centers values
			diffChange = updateClusterCenters(srcImage, _numClusters, clustersCenters, ElementsOfKthCluster, oldCenter, newCenter);
		}
		// Lưu ra ảnh đích
		for (int k = 0; k<_numClusters; k++)
		{
			vector<Point> ClusterKth = ElementsOfKthCluster[k];
			int bin = 255 / _numClusters * k;
			//for each pixel in cluster change color to fit cluster
			for (int i = 0; i < ClusterKth.size(); i++)
				dstImage.at<uchar>(ClusterKth[i].x, ClusterKth[i].y) = bin;
		}

		if (dstImage.empty())
			return 0;
		return 1;
	}

	Kmean() { _numClusters = 2; }
	~Kmean() {}
};

