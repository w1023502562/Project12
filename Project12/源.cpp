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
	if (!capVideo.isOpened())     //isOpened()�����Ƶ�Ƿ�����������������1
	{
		std::cout << "���ɴ���Ƶ�ļ�" << std::endl;
		return -1;
	}
	while (1) {
		capVideo >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		if (cnt == 0) {
			//��һ֡����ñ���ͼ��
			frame.copyTo(bgMat);

		}
		else {
			//�ڶ�֡��ʼ�������
			//����ͼ��͵�ǰͼ�����
			absdiff(frame, bgMat, subMat);
			//��ֽ����ֵ��
			threshold(subMat, bny_subMat, 50, 255, CV_THRESH_BINARY);

			imshow("b_subMat", bny_subMat);
			imshow("subMat", subMat);
			waitKey(30);
		}
		cnt = cnt + 1;
	}

	return 0;
}