#include "ColorTransformer.h"


int ColorTransformer::ChangeBrightness(const Mat & sourceImage, Mat & destinationImage, uchar b)
{
	//Tạo bảng lookup
	uchar lookup[256];
	for (int i = 0; i < 256; i++)
		lookup[i] = saturate_cast<uchar>(i + b);

	//Khởi tạo ảnh đích có kích thước và type giống ảnh nguồn
	destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

	//width là chiều rộng ảnh, height là chiều cao ảnh.
	int width = sourceImage.cols, height = sourceImage.rows;
	//nChannels là số kênh màu
	int nChannels = sourceImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = sourceImage.step[0];
	//pData là con trỏ quản lý vùng nhớ ảnh
	uchar* pData = (uchar*)destinationImage.data; //COn tro data cua anh dich
	uchar* psData = (uchar*)sourceImage.data; //COn tro data cua anh nguon
	for (int i = 0; i < height; i++, psData += widthStep, pData += widthStep) {
		uchar * pRow = pData; //Con tro dong cua anh dich
		uchar * psRow = psData;//Con tro dong cua anh nguon
		for (int j = 0; j < width; j++, pRow += nChannels, psRow += nChannels) {
			for (int k = 0; k < nChannels; k++)
				pRow[k] = lookup[(int)psRow[k]];
		}
	}
	if (destinationImage.empty())
		return 0; //Loi 
	return 1;
}

int ColorTransformer::ChangeContrast(const Mat & sourceImage, Mat & destinationImage, float c)
{
	uchar lookup[256];
	for (int i = 0; i < 256; i++)
		lookup[i] = saturate_cast<uchar>(i * c);

	//Khởi tạo ảnh đích có kích thước và type giống ảnh nguồn
	destinationImage.create(sourceImage.rows, sourceImage.cols, sourceImage.type());

	int width = sourceImage.cols, height = sourceImage.rows;
	//nChannels là số kênh màu
	int nChannels = sourceImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = sourceImage.step[0];
	//pData là con trỏ quản lý vùng nhớ ảnh
	uchar* pData = (uchar*)destinationImage.data; //COn tro data cua anh dich
	uchar* psData = (uchar*)sourceImage.data; //COn tro data cua anh nguon
	for (int i = 0; i < height; i++, psData += widthStep, pData += widthStep) {
		uchar * pRow = pData; //Con tro dong cua anh dich
		uchar * psRow = psData;//Con tro dong cua anh nguon
		for (int j = 0; j < width; j++, pRow += nChannels, psRow += nChannels) {
			for (int k = 0; k < nChannels; k++)
				pRow[k] = lookup[(int)psRow[k]];
		}
	}
	if (destinationImage.empty())
		return 0; //Loi 
	return 1;
}

int ColorTransformer::HistogramEqualization(const Mat & sourceImage, Mat & destinationImage)
{
	Mat src, hsv, hist;
	Converter con;
	src = sourceImage.clone();

	if (src.channels() != 1)
	{
		// If source image is not grayscale, convert it (RGB) to HSV
		con.Convert(src, hsv, 2);
		CalcHistogram(hsv, hist);
	}
	else
	{
		CalcHistogram(src, hist);
	}


	for (int bin = 1; bin < hist.cols; bin++)
		hist.at<signed>(2, bin) += hist.at<signed>(2, bin - 1);

	for (int bin = 0; bin < hist.cols; bin++)
		hist.at<signed>(2, bin) = (255.0 / (sourceImage.rows*sourceImage.cols))*hist.at<signed>(2, bin) + 0.5;

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			if (src.channels() == 1)
				src.at<uchar>(y, x) = saturate_cast<uchar>(hist.at<signed>(2, src.at<uchar>(y, x)));
			else
				hsv.at<Vec3b>(y, x)[2] = saturate_cast<uchar>(hist.at<signed>(2, src.at<Vec3b>(y, x)[2]));
		}
	}

	if (src.channels() != 1)
		// Convert res image (HSV) to RGB
		con.Convert(hsv, destinationImage, 3);
	else
		destinationImage = src.clone();

	return 1;
}

int ColorTransformer::CalcHistogram(const Mat & sourceImage, Mat & histogram)
{
	histogram = Mat::zeros(3, 256, CV_32S);

	int width = sourceImage.cols, height = sourceImage.rows;
	//nChannels là số kênh màu
	int nChannels = sourceImage.channels();
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int widthStep = sourceImage.step[0];
	//pData là con trỏ quản lý vùng nhớ ảnh
	uchar *pData = (uchar *)sourceImage.data;
	for (int i = 0; i < height; i++, pData += widthStep)
	{
		uchar *pRow = pData;
		for (int j = 0; j < width; j++, pRow += nChannels)
		{
			histogram.at<signed>(2, pRow[2])++;
			if (sourceImage.channels() != 1)
			{
				histogram.at<signed>(1, pRow[1])++;
				histogram.at<signed>(0, pRow[0])++;
			}
		}
	}

	for (int i = 0; i < histogram.cols; i++)
	{
		printf("%d %d %d\t", histogram.at<signed>(0, i), histogram.at<signed>(1, i), histogram.at<signed>(2, i));
	}
	printf("\n");

	return 1;
}

int ColorTransformer::DrawHistogram(const Mat & sourceImage, Mat & histImage)
{
	Mat histogram;
	histImage = Mat::zeros(480, 640, CV_8UC3);

	signed max_b = 0, max_g = 0, max_r = 0;
	float coeff[3];
	uchar color[3][3] = { 255, 0, 0, 0, 255, 0, 0, 0, 255 };

	CalcHistogram(sourceImage, histogram);

	/// Find the maximum bin of each channel
	for (int i = 0; i < histogram.cols; i++)
	{
		max_r = max(max_r, histogram.at<signed>(2, i));
		if (sourceImage.channels() != 1)
		{
			max_g = max(max_g, histogram.at<signed>(1, i));
			max_b = max(max_b, histogram.at<signed>(0, i));
		}
	}

	/// The coefficient that multiply with value of each bin
	/// to ensure the maximum height is the height of @histImage
	coeff[2] = float(histImage.rows) / max_r;
	if (sourceImage.channels() != 1)
	{
		coeff[1] = float(histImage.rows) / max_g;
		coeff[0] = float(histImage.rows) / max_b;
	}

	// Have 256 bins, so the distance between 2 bins is @histImage.cols / 255
	float binWidth = float(histImage.cols) / (255);

	for (int channel = 0; channel < histogram.rows; channel++)
	{
		/// Cordinate of the 1st bin in @histImage
		float prev_x = 0;
		float prev_y = histImage.rows - histogram.at<signed>(channel, 0)*coeff[channel];

		for (int bin = 1; bin < histogram.cols; bin++)
		{
			float x = binWidth * bin;
			float y = histImage.rows - histogram.at<signed>(channel, bin)*coeff[channel];
			Vec3b color3b;
			if (sourceImage.channels() != 1)
				color3b = Vec3b(color[channel][0], color[channel][1], color[channel][2]);
			else
				color3b = Vec3b(255, 255, 255);

			// Draw line connect current point with previous point
			line(histImage, Point(prev_x, prev_y), Point(x, y), color3b, 2, 8);
			// Update previous point
			prev_x = x;
			prev_y = y;
		}
	}

	return 1;
}

ColorTransformer::ColorTransformer()
{
}

ColorTransformer::~ColorTransformer()
{
}
