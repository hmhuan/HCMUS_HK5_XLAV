#include "CannyEdgeDetector.h"
#include<string>

int main(int argc, char * argv[])
{
	Mat srcImage, dstImage; //Ma trận lưu trữ ảnh nguồn và ảnh đích
	CannyEdgeDetector canny; // Đối tượng canny để thực thi tìm biên cạnh bằng pp.Canny

	string command(argv[1]); // lệnh nhập từ tham số
	string fileIn(argv[2]); //Input path
	string fileOut(argv[3]); //Output path
	int result = 0;
	int lowThreshold = atoi(argv[4]), upThreshold = atoi(argv[5]); //ngưỡng dưới và ngưỡng trên
	
	//Đọc ảnh với định dạng gốc (ảnh màu hay ảnh xám)
	srcImage = imread(fileIn, -1);
	if (command == "--canny") {
		canny.setThreshold(lowThreshold, upThreshold);
		result = canny.Apply(srcImage, dstImage);
	}
	if (!srcImage.empty())
		imshow("Original Image", srcImage);

	if (result) {
		imshow("Canny Edge Image", dstImage);
		imwrite(fileOut, dstImage);
	}
	waitKey(0);

	/*cout << fastAtan2(2.0, 1.0) << endl; 
	cout << atan(-2.0 / 1.0)*180/M_PI << endl;*/
	return 0;
}