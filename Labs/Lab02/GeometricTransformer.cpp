#include "GeometricTransformer.h"


PixelInterpolate::PixelInterpolate()
{
}

PixelInterpolate::~PixelInterpolate()
{
}

//Các hàm của lớp BilinearInterpolate

void BilinearInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels, uchar * pDstRow)
{
	int l = round(tx), r = round(ty);
	float a = tx - l, b = ty - r;
	uchar * pSrcRow0 = pSrc + (l * srcWidthStep + r * nChannels); // f(l, r)
	uchar * pSrcRow1 = pSrc + ((l + 1) * srcWidthStep + r * nChannels); // f(l + 1, r)
	uchar * pSrcRow2 = pSrc + (l * srcWidthStep + (r + 1) * nChannels); // f (l, r + 1)
	uchar * pSrcRow3 = pSrc + ((l+1) * srcWidthStep + (r+1) * nChannels); // f(l + 1, r + 1)
	for (int i = 0; i < nChannels; i++)
		pDstRow[i] = saturate_cast<uchar>((1 - a)*(1 - b)*pSrcRow0[i] + a*(1 - b)*pSrcRow1[i] + b*(1 - a)*pSrcRow2[i] + a*b*pSrcRow3[i]);
}

BilinearInterpolate::BilinearInterpolate()
{
}

BilinearInterpolate::~BilinearInterpolate()
{
}

//Các hàm của lớp NearestNeighborInterpolate

void NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels, uchar * pDstRow)
{
	int x =round(tx), y = round(ty);
	uchar * pSrcRow = pSrc + (x * srcWidthStep + y * nChannels);
	
	for (int i = 0; i < nChannels; i++)
		pDstRow[i] = pSrcRow[i];
}

NearestNeighborInterpolate::NearestNeighborInterpolate()
{
}

NearestNeighborInterpolate::~NearestNeighborInterpolate()
{
}

//Các hàm của lớp AffineTransform

void AffineTransform::Translate(float dx, float dy)
{
	//Gán các giá trị dx, dy tại (0, 2), (1, 2)
	Mat temp;
	temp.create(3, 3, CV_32FC1);
	//Khởi tạo matrix 3x3 với [1, 0, 0]
	//						  [0, 1, 0]
	//						  [0, 0, 1]	
	int width = temp.cols, height = temp.rows;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (x == y)
				temp.at<float>(x, y) = 1;
			else
				temp.at<float>(x, y) = 0;
	
	temp.at<float>(0, 2) = dx;
	temp.at<float>(1, 2) = dy;

	_matrixTransform = _matrixTransform * temp;
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			std::cout << _matrixTransform.at<float>(i, j) << " ";
		std::cout << "\n";
	}
	std::cout << "\n";

}

void AffineTransform::Rotate(float angle)
{
	//Tính sinA, cosA
	angle = angle * M_PI / 180;
	float sinAlpha = sinf(angle), cosAlpha = cosf(angle);
	//Gán các giá trị cosA, -sinA, sinA, cosA  tại (0, 0), (0, 1), (1, 0), (1, 1)
	_matrixTransform.at<float>(0, 0) = cosAlpha;
	_matrixTransform.at<float>(0, 1) = -sinAlpha;
	_matrixTransform.at<float>(1, 0) = sinAlpha;
	_matrixTransform.at<float>(1, 1) = cosAlpha;

	for (int i = 0; i < 3; i++)
	{
	for (int j = 0; j < 3; j++)
	std::cout << _matrixTransform.at<float>(i, j) << " ";
	std::cout << "\n";
	}
	std::cout << "\n";
}

void AffineTransform::Scale(float sx, float sy)
{
	//Gán các giá trị sx, sy tại (0, 0), (1, 1)
	_matrixTransform.at<float>(0, 0) = sx;
	_matrixTransform.at<float>(1, 1) = sy;
}
		
void AffineTransform::TransformPoint(float & x, float & y)
{
	float P[3] = { x, y, 1 }, P1[3] = {0, 0, 0}; //Tạo 2 ma trận điểm ảnh nguồn và đích
	//Nhân ma trận nguồn với ma trận chuyển đổi
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			P1[i] += P[j] * _matrixTransform.at<float>(i, j);
	//Gán lại giá trị mới cho điểm ảnh
	x = P1[0];
	y = P1[1];
}

AffineTransform::AffineTransform()
{
	//Khởi tạo matrix 3x3 type 32F chanels = 1;
	_matrixTransform.create(3, 3, CV_32FC1);
	//Khởi tạo matrix 3x3 với [1, 0, 0]
	//						  [0, 1, 0]
	//						  [0, 0, 1]	
	int width = _matrixTransform.cols, height = _matrixTransform.rows;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (x == y)
				_matrixTransform.at<float>(x, y) = 1;
			else
				_matrixTransform.at<float>(x, y) = 0;
}

AffineTransform::~AffineTransform()
{
}


//Các hàm của lớp GeometricTransformer

int GeometricTransformer::Transform(const Mat & beforeImage, Mat & afterImage, AffineTransform * transformer, PixelInterpolate * interpolator)
{
	//width là chiều rộng ảnh đích, height là chiều cao ảnh đích.
	//swidth là chiều rộng ảnh nguồn, height là chiều cao ảnh nguồn.
	int width = afterImage.cols, height = afterImage.rows;
	int swidth = beforeImage.cols, sheight = beforeImage.rows;
	//nChannels là số kênh màu
	int nChannels = beforeImage.channels();
	//swidthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp của ảnh nguồn
	//widthStep là khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp của ảnh đích
	int swidthStep = beforeImage.step[0];
	int widthStep = afterImage.step[0];
	//pData là con trỏ quản lý vùng nhớ ảnh đích
	uchar* pData = (uchar*)afterImage.data; 
	// psData là con trỏ quản lý vùng nhớ ảnh nguồn
	uchar* psData = (uchar*)beforeImage.data;
	float x, y;
	for (int i = 0; i < height; i++, pData += widthStep) 
	{
		uchar * pRow = pData; //Con trỏ dòng của ảnh đích
		for (int j = 0; j < width; j++, pRow += nChannels) 
		{
			x = i; y = j;
			transformer->TransformPoint(x, y); //Tính ra giá trị của p
			if (round(x) < sheight && round(y) < swidth && x >= 0 && y >= 0)
				interpolator->Interpolate(x, y, psData, swidthStep, nChannels, pRow);
			else
			{
				for (int k = 0; k < nChannels; k++)
					pRow[k] = 0;
			}

		}
	}

	return 1;
}

int GeometricTransformer::RotateKeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	Mat temp;
	//Khởi tạo đối tượng affineTf và gọi phương thức Rotate
	AffineTransform * affineTf = new AffineTransform();
	AffineTransform * affineTf1 = new AffineTransform();
	
	float angle1 = angle * M_PI / 180;
	int width = srcImage.cols, height = srcImage.rows;
	//int dstWidth = width * fabs(cosf(angle1)) + height * fabs(sinf(angle1));
	//int dstHeight = width * fabs(sinf(angle1)) + height * fabs(cosf(angle1));

	int dstWidth = height * 2;
	int dstHeight = width * 2;
	
	//Khởi tạo ảnh đích có kích thước và type giống ảnh nguồn
	temp.create(height * 2, width * 2, srcImage.type()); ///edit ????
	dstImage.create(height * 2, width * 2, srcImage.type());
	
	affineTf1->Translate(-height / 2, -width / 2);
	Transform(srcImage, temp, affineTf1, interpolator);
	imshow("Temp", temp);
	//waitKey(0);
	affineTf->Rotate(-angle);
	//affineTf->Translate(-fabs((dstHeight*sinf(angle1) - dstWidth*cosf(angle1))/2), 0); ///???
	affineTf->Translate(-height * sinf(angle1), 0);
	Transform(srcImage, dstImage, affineTf, interpolator);
	return 1;
}

int GeometricTransformer::RotateUnkeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	AffineTransform * affineTf = new AffineTransform();
	AffineTransform * affineTf1 = new AffineTransform();
	Mat temp;
	//Khởi tạo ảnh đích có kích thước và type giống ảnh nguồn
	
	dstImage.create(srcImage.rows,srcImage.cols, srcImage.type());

	affineTf->Rotate(-angle);
	affineTf->Translate(-125, 125); ///???
	Transform(srcImage, dstImage, affineTf, interpolator);
	return 1;
}

int GeometricTransformer::Scale(const Mat & srcImage, Mat & dstImage, float sx, float sy, PixelInterpolate * interpolator)
{
	//Khởi tạo đối tượng affineTf và gọi phương thức Scale
	AffineTransform * affineTf = new AffineTransform();
	affineTf->Scale(1.0f / sx, 1.0f / sy);

	//Khởi tạo ảnh đích có kích thước và type giống ảnh nguồn
	dstImage.create(round(srcImage.rows * sx), round(srcImage.cols * sy), srcImage.type());

	return Transform(srcImage, dstImage, affineTf, interpolator);
}

GeometricTransformer::GeometricTransformer()
{
}

GeometricTransformer::~GeometricTransformer()
{
}
