#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int calcGaussianBackground(vector<Mat> srcMats, Mat & meanMat, Mat &varMat)//遍历像素取高斯分布的参数
{

	int rows = srcMats[0].rows;
	int cols = srcMats[0].cols;


	for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{

			int sum = 0;
			float var = 0;
			//求均值
			for (int i = 0; i < srcMats.size(); i++) {
				sum += srcMats[i].at<uchar>(h, w);
			}
			meanMat.at<uchar>(h, w) = sum / srcMats.size();
			//求方差
			for (int i = 0; i < srcMats.size(); i++) {
				var += pow((srcMats[i].at<uchar>(h, w) - meanMat.at<uchar>(h, w)), 2);
			}
			varMat.at<float>(h, w) = var / srcMats.size();
		}
	}

	return 0;
}


int gaussianThreshold(Mat srcMat, Mat meanMat, Mat varMat, float weight, Mat & dstMat)//遍历每一帧跟参数对比

{
	int dstI;
	int rows = srcMat.rows;
	int cols = srcMat.cols;

	for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{
			int dif = abs(srcMat.at<uchar>(h, w) - meanMat.at<uchar>(h, w));
			int th = weight * varMat.at<float>(h, w);

			if (dif > th) {

				dstMat.at<uchar>(h, w) = 255;
			}
			else {
				dstMat.at<uchar>(h, w) = 0;
			}
		}
	}

	return 0;
}


int main() {

	//----------------------读取视频文件--------------------------
	VideoCapture capVideo(0);


	//如果视频打开失败
	if (!capVideo.isOpened()) {
		std::cout << "Unable to open video!" << std::endl;
		return -1;
	}

	//用来计算背景模型的图像
	std::vector<cv::Mat> srcMats;


	//参数设置
	int nBg = 200;		//用来建立背景模型的数量
	float wVar = 1;		//方差权重

	int cnt = 0;
	cv::Mat frame;
	cv::Mat meanMat;
	cv::Mat varMat;
	cv::Mat dstMat;

	while (true)
	{
		capVideo >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		//前面的nBg帧，计算背景
		if (cnt < nBg) {

			srcMats.push_back(frame);

			if (cnt == 0) {
				std::cout << "reading frame " << std::endl;
			}

		}
		else if (cnt == nBg) {
			//计算模型
			meanMat.create(frame.size(), CV_8UC1);
			varMat.create(frame.size(), CV_32FC1);
			std::cout << "calculating background models" << std::endl;
			calcGaussianBackground(srcMats, meanMat, varMat);
		}
		else {
			//背景差分
			dstMat.create(frame.size(), CV_8UC1);
			gaussianThreshold(frame, meanMat, varMat, wVar, dstMat);
			imshow("result", dstMat);
			imshow("frame", frame);
			waitKey(30);
		}

		cnt++;

	}

	return 0;
}