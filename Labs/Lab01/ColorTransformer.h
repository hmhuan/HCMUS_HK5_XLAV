#pragma once
#include "Converter.h"

class ColorTransformer
{
public:
	/*
	Hàm nhận vào một ảnh, thay đổi độ sáng của ảnh này và lưu kết quả vào ảnh mới
	Tham so:
		sourceImage	: ảnh ban đầu
		destinationImage: ảnh kết quả
		b	: giá trị số nguyên dùng để thay đổi độ sáng của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeBrightness(const Mat& sourceImage, Mat& destinationImage, uchar b);
	/*
	Hàm nhận vào một ảnh, thay đổi độ tương phản của ảnh này và lưu kết quả vào ảnh mới
	Tham so :
		sourceImage : ảnh ban đầu
		destinationImage : ảnh kết quả
		c	: giá trị số thực dùng để thay đổi độ tương phản của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/	
	int ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c);
	
	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		sourceImage : ảnh ban đầu
		destinationImage : ảnh kết quả
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int HistogramEqualization(const Mat& sourceImage, Mat& destinationImage);
	
	/*
	Hàm tính histogram cho ảnh
	Tham so :
		sourceImage : ảnh ban đầu có thể là ảnh xám hoặc ảnh màu
		histogram : histogram kết quả của ảnh input. 
					Nếu input là ảnh xám, histogram chỉ có 1 kênh
					Nếu input là ảnh màu, histogram có 3 kênh tương ứng theo 3 kênh của ảnh màu
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int CalcHistogram(const Mat& sourceImage, Mat& histogram);
	
	/*
	Hàm tính histogram cho ảnh
	Tham so :
		sourceImage : ảnh ban đầu có thể là ảnh xám hoặc ảnh màu
		histImage: ảnh histogram
					Nếu input là ảnh xám, chỉ vẽ 1 histogram
					Nếu input là ảnh màu, vẽ 3 histogram trên cùng 1 ảnh với màu tương ứng
						+ histogram của kênh Red vẽ bằng màu đỏ
						+ histogram của kênh Green vẽ bằng màu xanh lá
						+ histogram của kênh Blue vẽ bằng màu xanh dương
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int DrawHistogram(const Mat& sourceImage, Mat& histImage);
	
	ColorTransformer();
	~ColorTransformer();
};

