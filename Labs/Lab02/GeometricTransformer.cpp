#include "GeometricTransformer.h"

PixelInterpolate::PixelInterpolate()
{
}

PixelInterpolate::~PixelInterpolate()
{
}

void BilinearInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels, uchar * pDstRow)
{
}

BilinearInterpolate::BilinearInterpolate()
{
}

BilinearInterpolate::~BilinearInterpolate()
{
}

void NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels, uchar * pDstRow)
{
}

NearestNeighborInterpolate::NearestNeighborInterpolate()
{
}

NearestNeighborInterpolate::~NearestNeighborInterpolate()
{
}

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
