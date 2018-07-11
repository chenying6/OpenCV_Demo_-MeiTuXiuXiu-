/*************************************************
Copyright:CY
Author:Ying Chen
Date:2018-07-11
Description:Edit(tailor, rotation), Enhancement(brightness, contract, smoothing and sharpening), Mosaic, arbitrarily graffiti
**************************************************/

#include <iostream>
#include "beauty.h"
#include "mainInterface.h"
#include "opencv2\opencv.hpp"
#include "qwidget.h"
#include "qfiledialog.h"
#include "file.h"

using namespace cv;
using namespace std;


void on_mouse(int event, int x, int y, int flags, void *ustc);
void imageRotate(const cv::Mat &src, cv::Mat &dst, const double degree, const double scale);
void getImage(const cv::Mat &src, cv::Mat &dst, float height, float width, float degree);

void guangban_on_mouse(int event, int x, int y, int flags, void *ustc);

void createMosaicImage(Mat inputMat, Mat& outputMat, int size);
void setMosaic(Mat& inputMat, Rect rect);
void cleanMosaic(Mat& inputMat, Rect rect);
void Mosaic_onMouse(int events, int x, int y, int flag, void* ustg);

void Tuya_onMouse(int event, int x, int y, int flag, void *param);

Mat QImage2cvMat(QImage image);


Beauty::Beauty(QWidget *parent):QWidget(parent) {
	cout << "In beauty......";
}

Beauty:: ~Beauty() {

}

#pragma region Edit_Tailor_Rotate

Mat org, dst, img, tmp;
void Beauty::tailor() {
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	org = imread(fileName);
	org.copyTo(img);
	org.copyTo(tmp);
	namedWindow("img");//定义一个img窗口  
	setMouseCallback("img", on_mouse, 0);//调用回调函数  
	imshow("img", img);
	
	waitKey(0);
}
void on_mouse(int event, int x, int y, int flags, void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
{
	static Point pre_pt = (-1, -1);//初始坐标  
	static Point cur_pt = (-1, -1);//实时坐标  
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{
		org.copyTo(img);//将原始图片复制到img中  
		sprintf(temp, "(%d,%d)", x, y);
		pre_pt = Point(x, y);
		putText(img, temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);//在窗口上显示坐标  
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//划圆  
		imshow("img", img);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	{
		img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标  
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));//只是实时显示鼠标移动的坐标  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
	{
		img.copyTo(tmp);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(tmp, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形  
	{
		org.copyTo(img);
		sprintf(temp, "(%d,%d)", x, y);
		cur_pt = Point(x, y);
		putText(img, temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//根据初始点和结束点，将矩形画到img上  
		imshow("img", img);
		img.copyTo(tmp);
		//截取矩形包围的图像，并保存到dst中  
		int width = abs(pre_pt.x - cur_pt.x);
		int height = abs(pre_pt.y - cur_pt.y);
		if (width == 0 || height == 0)
		{
			printf("width == 0 || height == 0");
			return;
		}
		dst = org(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));
		namedWindow("dst");
		imshow("dst", dst);

		waitKey(0);
	}
}

void Beauty::rotate() {
	//opencv实现旋转90、-90、180度方法
	//顺时针旋转90度-90（transpose+flip0）
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	Mat img = imread(fileName);
	/*transpose(img, img);
	flip(img, img, 0);
	imshow("-90", img);
	waitKey();
	//逆时针旋转90度+90（transpose+flip1）
	Mat img2 = imread(fileName);
	transpose(img2, img2);
	flip(img2, img2, 1);
	imshow("+90", img2);
	waitKey();
	//旋转180度
	Mat img3 = imread(fileName);
	flip(img3, img3, -1);
	imshow("180", img3);
	waitKey();*/
	//旋转任意角度，需要调用imageRotate函数
	Mat img4 = imread(fileName);
	double degree = 30;
	imageRotate(img4, img4, degree, 1);
	//getImage(img4, img4, 1000, 800, abs(degree));
	imshow("30", img4);
	waitKey();
}

//定义一张边长为原图像对角线长度的正方形图像，将图片通过仿射变化进行旋转，注意还可以定义缩放比例
//角度设置：顺时针为负，逆时针为正  degree为角度，不是弧度
void imageRotate(const Mat &src, Mat &dst, const double degree, const double scale)
{
	double angle = degree;
	int length = 0;//保证旋转后的图像足够大能存放结果（对角线长度）
	if (scale <= 1)
		length = sqrt(src.cols*src.cols + src.rows*src.rows);
	else
		length = sqrt(src.cols*src.cols + src.rows*src.rows) * scale;
	Mat tmp_img = Mat(length, length, src.type(), Scalar(255, 255, 255));
	int ROI_X = length / 2 - src.cols / 2;
	int ROI_Y = length / 2 - src.rows / 2;
	Rect ROI_SRC(ROI_X, ROI_Y, src.cols, src.rows);
	Mat tmp_ROI(tmp_img, ROI_SRC);//取出临时图像tmp_img的中心的一块ROI_SEC存放src，也就是将src图像放到了临时图像tmp_img的中心
	src.copyTo(tmp_ROI);
	Point2f rotate_center(length / 2, length / 2);//旋转中心
	Mat affineMat = getRotationMatrix2D(rotate_center, angle, scale);//仿射变换旋转矩阵
	warpAffine(tmp_img, dst, affineMat, Size(length, length), INTER_CUBIC, 0, Scalar(255, 255, 255));//仿射变换
}
//src上一步旋转得到的图像；dst取出有信息部分图像；height和width为原图的长和宽；degree旋转的角度的绝对值
void getImage(const Mat &src, Mat &dst, float height, float width, float degree)
{
	degree = degree * CV_PI / 180;
	float height1, width1;

	height1 = width * sin(degree) + height * cos(degree);
	width1 = width * cos(degree) + height * sin(degree);

	if (height1 > src.rows)
		height1 = src.rows;
	if (width1 > src.cols)
		width1 = src.cols;
	Point2f point(src.cols / 2 - width1 / 2, src.rows / 2 - height1 / 2);
	Rect2f rect(point, Size2f(width1, height1));
	Mat temp = src(rect);
	dst = temp;
}
void Beauty::saveFromEdit() {
	file->WriteData("D:\\savedPhoto.jpg", img);
	file->WriteData("D:\\savedPhoto.jpg", dst);
}

#pragma endregion

#pragma region Enhance_liangdu_duibidu_ruihua_baohedu
void Beauty::liangdu(int value) {
	cout << "liangdu_slider:  " << value << endl;
	double alpha = 1, beta = 0;
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	Mat photo = imread(fileName);

	cout << "In liangdu......" << endl;
	//imshow("LiangDu", photo);
	Mat dst = Mat::zeros(photo.size(), photo.type());

	beta = value;

	for (int y = 0; y < photo.rows; y++) {
		for (int x = 0; x < photo.cols; x++) {
			for (int c = 0; c < 3; c++) {
				dst.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(alpha*(photo.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}
	imshow("LiangDuModified", dst);

}


void Beauty::duibidu(int value) {
	cout << "duibidu_slider:  " << value << endl;
	double alpha = 1, beta = 0;
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	Mat photo = imread(fileName);

	cout << "In duibidu......" << endl;
	//imshow("LiangDu", photo);
	Mat dst = Mat::zeros(photo.size(), photo.type());

	beta = 0;
	alpha = (value + 150) / 100.0;//加100的时候，最低的对比度直接变为0了

	for (int y = 0; y < photo.rows; y++) {
		for (int x = 0; x < photo.cols; x++) {
			for (int c = 0; c < 3; c++) {
				dst.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(alpha*(photo.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}
	imshow("DuiBiDuModified", dst);
}
void Beauty::ruihua(int value) {
	cout << "ruihua_slider:  " << value << endl;
	double alpha = 1, beta = 0;
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	Mat photo = imread(fileName);
	Mat dst = Mat::zeros(photo.size(), photo.type());
	float kr[9] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };
	float kp[9] = { 1,1,1,1,1,1,1,1,1 };
	if (value <= -30) {
		medianBlur(photo, dst, 3);
		imshow("RuiHuaModified", dst);
		cout << "In ruihua......" << endl;
		//imshow("LiangDu", photo);
	}
	else if (value >= 30) {
		Mat kernel(3, 3, CV_32F, kr);
		filter2D(photo, dst, photo.depth(), kernel);
		imshow("RuiHuaModified", dst);
		cout << "In ruihua......" << endl;
		//imshow("LiangDu", photo);
	}
	else {
		photo.copyTo(dst);
		imshow("RuiHuaModified", dst);
		cout << "In ruihua......" << endl;
		//imshow("LiangDu", photo);
	}

}
void Beauty::baohedu(int value) {
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	Mat photo = imread(fileName);
	Mat dst = Mat::zeros(photo.size(), photo.type());
	/*cvtColor(photo, dst, COLOR_RGB2HSV);
	vector<Mat> channels;
	split(dst, channels);
	for (int i = 0; i < 2; i++) {
	for (int j = 0; j < channels[1].cols; j++) {
	dst.at<Vec3b>(i, j)[1] += value/100.0;
	dst.at<Vec3b>(i, j)[1] /= 2.0;
	cout << dst.at<Vec3b>(i, j)[1] << " ";
	}
	cout << endl;
	}
	merge(channels, dst);
	cvtColor(dst, dst, COLOR_HSV2RGB);*/


	imshow("BaoHeDu", dst);

}
void Beauty::saveFromEnhance() {
	file->WriteData("D:\\savedPhoto.jpg", dst);
}

#pragma endregion


#pragma region backgroundXuHua_guangban
Point2f rotate_center;
void Beauty::guangban(int value) {
	cout << "guangban_slider:  " << value << endl;
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	Mat src = imread(fileName);
	Mat dst = Mat::zeros(src.size(), src.type());
	rotate_center = Point2f(src.cols / 2, src.rows / 2);

	Mat roi = Mat::zeros(src.size(), CV_8UC1);
	Mat tmp;
	int guangbansize = 0;
	int max = src.cols > src.rows ? src.cols / 2 : src.rows / 2;
	guangbansize = (int)(((value + 100) / 200.0)*(max - 30)) + 30;

	//setMouseCallback("GuangBan", guangban_on_mouse, 0);
	circle(roi, rotate_center, guangbansize, Scalar(255, 255, 255), -1);
	subtract(src, src, tmp, roi);
	src.copyTo(tmp, roi);
	blur(src, dst, Size(11, 11));
	tmp.copyTo(dst, roi);
	circle(dst, rotate_center, guangbansize, Scalar(255, 255, 255), 0);
	imshow("GuangBan", dst);
	waitKey();
}
void guangban_on_mouse(int event, int x, int y, int flags, void *ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下 
	{
		rotate_center = Point(x, y);
		cout << x + " " << y << endl;
	}
}
void Beauty::saveFromBackground() {
	file->WriteData("D:\\savedPhoto.jpg", dst);
}

#pragma endregion



#pragma region Mosaic

Mat inputImage;
Mat inputImage_mosaic;
Mat inputImage_clone;
bool IsErase = false;
//马赛克的大小，可以调整
int neightbourhood = 10;
//记录鼠标的状态，0为鼠标左键未按下或弹起，1为鼠标左键按下
int mouseStatus = 0;
void Beauty::Mosaic() {
	IsErase = false;
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	inputImage = imread(fileName);
	inputImage_clone = inputImage.clone();
	createMosaicImage(inputImage, inputImage_mosaic, neightbourhood);

	namedWindow("showImage", 0);
	setMouseCallback("showImage", Mosaic_onMouse);
	waitKey();
}
void Beauty::PainterMosaic(int value) {
	neightbourhood = 10 + value / 10;
}
void Beauty::eraseMosaic() {
	IsErase = true;
}

void createMosaicImage(Mat inputMat, Mat& outputMat, int size) {
	RNG rng;
	int height = inputMat.rows;
	int width = inputMat.cols;
	Mat padding;
	Mat tempMat;

	//为了方便后面的计算，将输入的图像大小扩充到宽高都是size的倍数
	copyMakeBorder(inputMat, padding, 0, size - inputMat.rows % size, 0, size - inputMat.cols % size, BORDER_REPLICATE);
	tempMat = padding.clone();

	for (int row = 0; row < padding.rows; row += size) {
		for (int col = 0; col < padding.cols; col += size) {
			int rand_x = rng.uniform(0, size);
			int rand_y = rng.uniform(0, size);
			Rect rect = Rect(col, row, size, size);
			Mat roi = tempMat(rect);
			Scalar color = Scalar(padding.at<Vec3b>(row + rand_y, col + rand_x)[0], \
				padding.at<Vec3b>(row + rand_y, col + rand_x)[1], \
				padding.at<Vec3b>(row + rand_y, col + rand_x)[2]);
			roi.setTo(color);
		}
	}
	outputMat = tempMat(Rect(0, 0, width, height)).clone();
}
//加马赛克
void setMosaic(Mat& inputMat, Rect rect) {
	Mat roi = inputMat(rect);
	Mat tempRoi = inputImage_mosaic(rect);
	tempRoi.copyTo(roi);
}
//清除马赛克
void cleanMosaic(Mat& inputMat, Rect rect) {
	Mat roi = inputMat(rect);
	Mat tempRoi = inputImage(rect);
	tempRoi.copyTo(roi);
}

void Mosaic_onMouse(int events, int x, int y, int flag, void* ustg) {

	//当鼠标移除图片区域的时候，不做操作
	if (x < 0 || x > inputImage.cols || y < 0 || y > inputImage.rows) {
		return;
	}

	//马赛克块的位置信息
	int x_left, x_right, y_top, y_bottom;
	x - neightbourhood <= 0 ? x_left = 0 : x_left = x - neightbourhood;
	x + neightbourhood > inputImage.cols ? x_right = inputImage.cols : x_right = x + neightbourhood;
	y - neightbourhood <= 0 ? y_top = 0 : y_top = y - neightbourhood;
	y + neightbourhood > inputImage.rows ? y_bottom = inputImage.rows : y_bottom = y + neightbourhood;

	if (events == CV_EVENT_LBUTTONDOWN) {
		mouseStatus = 1;
		if (IsErase) {
			cleanMosaic(inputImage_clone, Rect(x_left, y_top, x_right - x_left, y_bottom - y_top));
		}
		else
			setMosaic(inputImage_clone, Rect(x_left, y_top, x_right - x_left, y_bottom - y_top));

	}
	else if (events == CV_EVENT_MOUSEMOVE) {
		if (mouseStatus == 1) {
			if (IsErase) {
				cleanMosaic(inputImage_clone, Rect(x_left, y_top, x_right - x_left, y_bottom - y_top));
			}
			else setMosaic(inputImage_clone, Rect(x_left, y_top, x_right - x_left, y_bottom - y_top));

		}
		else {
			//nothing
		}
	}
	else if (events == CV_EVENT_LBUTTONUP) {
		mouseStatus = 0;
	}
	else {
		//cout << "nothing" << endl;
	}
	imshow("showImage", inputImage_clone);
}
void Beauty::saveFromMosaic() {
	file->WriteData("D:\\savedPhoto.jpg", inputImage_mosaic);
}

#pragma endregion

#pragma region Tuya

Rect g_rectangle;
bool g_bDrawing = false;
RNG g_rng(12345);
Point g_nCurrPoint;
Mat srcImage;
int g_nThick = 0, g_nBlue = 255, g_nGreen = 255, g_nRed = 0;

void Beauty::Handwritting() {
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	Mat tempImage = imread(fileName);
	srcImage = imread(fileName);

	//tempImage.copyTo(srcImage);
	//srcImage = Scalar::all(0);
	namedWindow("TuYa");
	setMouseCallback("TuYa", Tuya_onMouse,0);
	char key;
	while (1)
	{
		if (g_bDrawing)
		{
			circle(srcImage, g_nCurrPoint, g_nThick, Scalar(g_nBlue, g_nGreen, g_nRed), -1,8);
		}

		imshow("TuYa", srcImage);

		key = waitKey(1);
		if (key == 27)
			break;

		if (key == '1')
			tempImage.copyTo(srcImage);

	}
}
void Tuya_onMouse(int event, int x, int y, int flag, void *param)
{
	Mat &img = *(cv::Mat*)param;

	switch (event)
	{
		//移动鼠标的时候
	case CV_EVENT_MOUSEMOVE:
	{
		g_nCurrPoint = Point(x, y);
	}
	break;
	//点击鼠标左键时
	case CV_EVENT_LBUTTONDOWN:
	{
		g_bDrawing = true;
		g_nCurrPoint = Point(x, y);
	}
	break;
	//松开鼠标左键时
	case CV_EVENT_LBUTTONUP:
	{
		g_bDrawing = false;
	}
	break;
	}
}

void Beauty::setblue(int value) {
	g_nBlue = value;
}
void Beauty::setred(int value) {
	g_nRed = value;
}
void Beauty::setgreen(int value) {
	g_nGreen = value;
}
void Beauty::setthickness(int value) {
	g_nThick = 1 + (value + 100) / 10;
}
void Beauty::restore() {
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	Mat tempImage = imread(fileName);
	imshow("TuYa", tempImage);
}

void Beauty::saveFromTuYa() {
	file->WriteData("D:\\savedPhoto.jpg", srcImage);
}
#pragma endregion

#pragma region Writeword
void Beauty::Writeword() {

}
#pragma endregion



Mat QImage2cvMat(QImage image)
{

	cv::Mat mat;
	switch (image.format())

	{

	case QImage::Format_ARGB32:

	case QImage::Format_RGB32:

	case QImage::Format_ARGB32_Premultiplied:

		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());

		break;

	case QImage::Format_RGB888:

		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());

		cv::cvtColor(mat, mat, CV_BGR2RGB);

		break;

	case QImage::Format_Indexed8:

		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());

		break;

	}

	return mat;

}


