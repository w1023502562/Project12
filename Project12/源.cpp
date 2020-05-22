#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;



int main() {
	int cnt = 0;
	Mat frame;
	Mat bgMat;
	Mat subMat;
	Mat bny_subMat;
	VideoCapture capVideo;
	capVideo.open(0);
	if (!capVideo.isOpened())     //isOpened()检查视频是否开启，正常开启返回1
	{
		std::cout << "不可打开视频文件" << std::endl;
		return -1;
	}
	while (1) {
		capVideo >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		if (cnt == 0) {
			//第一帧，获得背景图像
			frame.copyTo(bgMat);

		}
		else {
			//第二帧开始背景差分
			//背景图像和当前图像相减
			absdiff(frame, bgMat, subMat);
			//差分结果二值化
			threshold(subMat, bny_subMat, 50, 255, CV_THRESH_BINARY);

			imshow("b_subMat", bny_subMat);
			imshow("subMat", subMat);
			waitKey(30);
		}
		cnt = cnt + 1;
	}

	return 0;
}