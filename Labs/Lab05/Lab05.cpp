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
		int size = atoi(argv[3]), k = atoi(argv[4]), r = atoi(argv[5]);
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
	/*Mat test;
	test = Mat::zeros(5, 5, CV_32SC1);
	int *pData = (int *)test.data;
	pData = pData - 12;
	int * pRow;
	for (int i = -2; i <= 6; i++, pData += 5)
	{
		pRow = pData;
		for (int j = -2; j <= 6; j++, pRow += 1)
			cout << pRow[0] << " ";
		cout << endl;
	}*/
	return 0;
}