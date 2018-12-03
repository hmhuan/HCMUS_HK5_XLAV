#include "ColorTransformer.h"


int ColorTransformer::ChangeBrightness(const Mat & sourceImage, Mat & destinationImage, uchar b)
{
	if (sourceImage.empty())
		return 0;

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
	if (sourceImage.empty())
		return 0;

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
	if (sourceImage.empty())
		return 0;

	Mat src, hsv, hist;
	Converter con;
	src = sourceImage.clone();

	if (src.channels() != 1)
	{
		// Nếu ảnh nguồn là là ảnh màu, chuyển sang HSV rồi tính histogram
		con.Convert(src, hsv, 2);
		CalcHistogram(hsv, hist);
	}
	else
	{
		CalcHistogram(src, hist);
	}

	if (sourceImage.channels() == 1)
	{
		// T[p] = T[p-1] + H[p]
		// H là lược đồ độ xám
		// T là lược đồ độ xám tích lũy
		for (int bin = 1; bin < hist.cols; bin++)
			hist.at<signed>(0, bin) += hist.at<signed>(0, bin - 1);

		// Chuẩn hóa T về đoạn [0-255]
		for (int bin = 0; bin < hist.cols; bin++)
			hist.at<signed>(0, bin) = (255.0 / (sourceImage.rows*sourceImage.cols))*hist.at<signed>(0, bin) + 0.5;
	}
	else
	{
		for (int bin = 1; bin < hist.cols; bin++)
			hist.at<signed>(2, bin) += hist.at<signed>(2, bin - 1);

		// Chuẩn hóa T về đoạn [0-255]
		for (int bin = 0; bin < hist.cols; bin++)
			hist.at<signed>(2, bin) = (255.0 / (sourceImage.rows*sourceImage.cols))*hist.at<signed>(2, bin) + 0.5;
	}

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			if (src.channels() == 1)
				// Thay giá trị màu của pixel (x, y) thành giá trị đã được cân bằng
				src.at<uchar>(y, x) = hist.at<signed>(0, src.at<uchar>(y, x));
			else
				// Thay giá trị màu tại pixel (x, y) của kênh V thành giá trị đã được cân bằng
				hsv.at<Vec3b>(y, x)[2] = hist.at<signed>(2, src.at<Vec3b>(y, x)[2]);
		}
	}

	if (src.channels() != 1)
		// Nếu là ảnh màu, do lúc đầu chuyển thành HSV nên giờ chuyển về lại RGB
		con.Convert(hsv, destinationImage, 3);
	else
		destinationImage = src.clone();

	return 1;
}

int ColorTransformer::CalcHistogram(const Mat & sourceImage, Mat & histogram)
{
	if (sourceImage.empty())
		return 0;
	if (sourceImage.channels() == 1)
		histogram = Mat::zeros(1, 256, CV_32S);
	else
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
			if (sourceImage.channels() == 1)
				histogram.at<signed>(0, pRow[0])++;
			else
			{
				histogram.at<signed>(2, pRow[2])++;
				histogram.at<signed>(1, pRow[1])++;
				histogram.at<signed>(0, pRow[0])++;
			}
		}
	}

	// for (int i = 0; i < histogram.cols; i++)
	// {
	// 	printf("%d %d %d\t", histogram.at<signed>(0, i), histogram.at<signed>(1, i), histogram.at<signed>(2, i));
	// }
	// cout << endl;

	return 1;
}

int ColorTransformer::DrawHistogram(const Mat & sourceImage, Mat & histImage)
{
	if (sourceImage.empty())
		return 0;

	Mat histogram;
	histImage = Mat::zeros(HIST_HEIGHT, HIST_WIDTH, CV_8UC3);

	signed max_b = 0, max_g = 0, max_r = 0;
	float coeff[3];
	uchar color[3][3] = { 255, 0, 0, 0, 255, 0, 0, 0, 255 };

	CalcHistogram(sourceImage, histogram);

	/// Find the maximum bin of each channel
	for (int i = 0; i < histogram.cols; i++)
	{
		// Nếu là ảnh xám thì đây là giá trị max của kênh màu ảnh xám
		max_b = max(max_b, histogram.at<signed>(0, i));
		if (sourceImage.channels() != 1)
		{
			max_r = max(max_r, histogram.at<signed>(2, i));
			max_g = max(max_g, histogram.at<signed>(1, i));
		}
	}

	/// The coefficient that multiply with value of each bin
	/// to ensure the maximum height is the height of @histImage
	coeff[0] = float(histImage.rows) / max_b;
	if (sourceImage.channels() != 1)
	{
		coeff[1] = float(histImage.rows) / max_g;
		coeff[2] = float(histImage.rows) / max_r;
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
