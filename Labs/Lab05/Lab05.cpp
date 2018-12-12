#include "Kmean.h"

int main(int argc, char * argv[])
{
	Mat srcImage, dstImage;
	int result = 0;
	
	string Command(argv[1]), InputPath(argv[2]);

	srcImage = imread(InputPath, -1);

	if (srcImage.empty())
		return 0;

	if (Command == "--static") //Phân ngưỡng toàn cục tĩnh
	{
		//đối số: size kích thước cửa sổ, hệ số ngưỡng trên và ngưỡng dưới
		int low = atoi(argv[3]), high = atoi(argv[4]);
		StaticThreshold _static;
		_static.setThreshold(low, high);
		result = _static.Apply(srcImage, dstImage);
	}
	else if (Command == "--mean") //Phân ngưỡng động theo trung bình
	{
		//đối số: size kích thước cửa sổ, hệ số C
		int size = atoi(argv[3]), C = atoi(argv[4]);
		AverageLocalThreshold _mean;
		_mean.setC(C);
		result = _mean.Apply(srcImage, dstImage, Size(size, size));
	}
	else if (Command == "--median") //Phân ngưỡng động theo trung vị
	{
		//đối số: size kích thước cửa sổ, hệ số C
		int size = atoi(argv[3]), C = atoi(argv[4]);
		MedianLocalThreshold _median;
		_median.setC(C);
		result = _median.Apply(srcImage, dstImage, Size(size, size));
	}
	else if (Command == "--sauvola") //Phân ngưỡng động Sauvola
	{
		//đối số: size:kích thước của sổ, hệ số k và r
		int size = atoi(argv[3]), r = atoi(argv[5]);
		float k = atof(argv[4]);
		SauvolaLocalThreshold sauvola;
		sauvola.setSauvola(k, r);
		result = sauvola.Apply(srcImage, dstImage, Size(size, size));
	}
	else if (Command == "--kmeans") //Phân đoạn k-means
	{
		//đối số: số cụm (clusters)
		int clusters = atoi(argv[3]);
		Kmean _kmean;
		_kmean.setNumClusters(clusters);
		result = _kmean.Apply(srcImage, dstImage);
	}

	imshow("Original  Image", srcImage);
	if (result)
	{
		imshow("Destination Image", dstImage);
	}
	waitKey(0);
	
	/*Mat test, or;
	
	or.create(5, 5, CV_8UC1);
	for (int i = 0; i < or.cols; i++)
	{
		for (int j = 0; j < or.rows; j++)
		{
			or.at<uchar>(i, j) = 1;
			cout << (int)or.at<uchar>(i, j) << " ";
		}
		cout << "\n";
	}
		
	createIntergral(or , test, 1, 1);
	for (int i = 0; i < test.rows; i++)
	{
		for (int j = 0; j < test.cols; j++)
			cout << test.at<float>(i, j) << " ";
		cout << "\n";
	}*/

	return 0;
}