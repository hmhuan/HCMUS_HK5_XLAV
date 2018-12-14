#include "SVD.h"

int main(int argc, char * argv[])
{
	// Các ma trận ảnh
	Mat srcImage, cvtImage, dstImage, W, U, Vt;
	string Command(argv[1]), InputPath, OutputPath;
	int result = 0, type;
	fstream fU, fW, fVt;
	FILE *fileU, *fileW, *fileVT;

	if (Command == "--compress")
	{
		InputPath = argv[2];
		// Đọc ảnh đầu vào
		srcImage = imread(InputPath, -1);

		if (!srcImage.empty())
			imshow("Original Image", srcImage);
		/*fileU = fopen(argv[3], "wb");
		fileW = fopen(argv[4], "wb");
		fileVT = fopen(argv[5], "wb");*/
		// Mở các file U, W, Vt để ghi ra file nhị phân
		fU.open(argv[3], ios::out | ios::binary);
		fW.open(argv[4], ios::out | ios::binary);
		fVt.open(argv[5], ios::out | ios::binary);
		int k = atoi(argv[6]);
		// Thực hiện nén ảnh và lưu ra các file
		//result = Compress(srcImage, fileU, fileW, fileVT, k);
		result = Compress(srcImage, fU, fW, fVt, k);
		fU.close();
		fW.close();
		fVt.close();
		/*fclose(fileU);
		fclose(fileVT);
		fclose(fileW);*/
	}
	else if (Command == "--decompress")
	{
		OutputPath = argv[5];
		fU.open(argv[2], ios::in | ios::binary);
		fW.open(argv[3], ios::in | ios::binary);
		fVt.open(argv[4], ios::in | ios::binary);

		type = 2;
		result = Decompress(fU, fW, fVt, dstImage);

		fU.close();
		fW.close();
		fVt.close();
		if (result)
		{
			//Nếu decompress hiển thị ảnh giải nén
			imshow("Decompress Image", dstImage);
			imwrite(OutputPath, dstImage);
		}
	}
	waitKey(0);

	return 0;
}
