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
}

void AffineTransform::Rotate(float angle)
{
}

void AffineTransform::Scale(float sx, float sy)
{
}

void AffineTransform::TransformPoint(float & x, float & y)
{
}

AffineTransform::AffineTransform()
{
}

AffineTransform::~AffineTransform()
{
}


//Các hàm của lớp GeometricTransformer

int GeometricTransformer::Transform(const Mat & beforeImage, Mat & afterImage, AffineTransform * transformer, PixelInterpolate * interpolator)
{
	return 0;
}

int GeometricTransformer::RotateKeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	return 0;
}

int GeometricTransformer::RotateUnkeepImage(const Mat & srcImage, Mat & dstImage, float angle, PixelInterpolate * interpolator)
{
	return 0;
}

int GeometricTransformer::Scale(const Mat & srcImage, Mat & dstImage, float sx, float sy, PixelInterpolate * interpolator)
{

	return 0;
}

GeometricTransformer::GeometricTransformer()
{
}

GeometricTransformer::~GeometricTransformer()
{
}
