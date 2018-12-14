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
	else if (Command == "--mean") //Phân ngưỡng cục bộ theo trung bình
	{
		//đối số: size kích thước cửa sổ, hệ số C
		int size = atoi(argv[3]), C = atoi(argv[4]);
		AverageLocalThreshold _mean;
		_mean.setC(C);
		result = _mean.Apply(srcImage, dstImage, Size(size, size));
	}
	else if (Command == "--median") //Phân ngưỡng cục bộ theo trung vị
	{
		//đối số: size kích thước cửa sổ, hệ số C
		int size = atoi(argv[3]), C = atoi(argv[4]);
		MedianLocalThreshold _median;
		_median.setC(C);
		result = _median.Apply(srcImage, dstImage, Size(size, size));
	}
	else if (Command == "--sauvola") //Phân ngưỡng cục bộ áp dụng Sauvola
	{
		//đối số: size:kích thước của sổ, hệ số k và r
		int size = atoi(argv[3]), r = atoi(argv[5]);
		float k = atof(argv[4]);
		SauvolaLocalThreshold sauvola;
		sauvola.setSauvola(k, r);
		result = sauvola.Apply(srcImage, dstImage, Size(size, size));
	}
	else if (Command == "--kmeans") //Phân đoạn ảnh bằng phương pháp k-means
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
	
	//Mat test, or, res;
	//
	//or.create(5, 5, CV_8UC1);
	//for (int i = 0; i < or.cols; i++)
	//{
	//	for (int j = 0; j < or.rows; j++)
	//	{
	//		or.at<uchar>(i, j) = i + j;
	//		cout << (int)or.at<uchar>(i, j) << " ";
	//	}
	//	cout << "\n";
	//}
	//	
	//createIntergral(or , test, 3, 1);
	//for (int i = 0; i < test.rows; i++)
	//{
	//	for (int j = 0; j < test.cols; j++)
	//		cout << test.at<float>(i, j) << " ";
	//	cout << "\n";
	//}
	//vector<int> offsets;
	//for (int y = -1; y <= 1; y++)
	//	for (int x = -1; x <= 1; x++)
	//		offsets.push_back(y * (test.cols)+x);
	//int TopBot[4];
	//TopBot[0] = offsets[0] + (-test.cols - 1); //TL
	//TopBot[1] = offsets[3 - 1] + (-(test.cols)); //TR
	//TopBot[2] = offsets[9 - 3] + (-1); //BL
	//TopBot[3] = offsets[9 - 1]; //BR
	//float *pT = (float*)test.data, *pR;
	//pT = pT + ((1 + 1) * test.cols + (1 + 1));
	//float average;
	//int widthStepI = test.cols;
	//for (int i = 0; i < 5; i++, pT += widthStepI)
	//{
	//	pR = pT;
	//	for (int j = 0; j < 5; j++, pR+= 1)
	//	{
	//		average = 0.0f;
	//		for (int k = 0; k < 4; k++)
	//		{
	//			if (k == 0 || k == 3)
	//				average += pR[TopBot[k]];
	//			else
	//				average -= pR[TopBot[k]];
	//		}
	//		/*for (int k = 0; k < n; k++)
	//		average += psRow[offsets[k]];*/
	//		average /= 9;
	//		cout << average << " ";
	//	}
	//	cout << endl;
	//}

	return 0;
}