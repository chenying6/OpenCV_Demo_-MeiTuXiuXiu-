#include "people.h"
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

void initWhite();
int FMax(const int X, const int Y);
int FMin(const int X, const int Y);
void BalanceColor(Mat& bitmap, int value);
//���ص�����
void Remove_OnMouse(int event, int x, int y, int flag, void *ustg);
//void BilinearInter(Mat& img, int statU, int startV, double alpha, double beta, uchar blue, uchar green, uchar red);
Mat BilinearInter(Mat& img);
void Slim_OnMouse(int event, int x, int y, int flag, void *ustg);
//���ص�����
void Heighten_OnMouse(int event, int x, int y, int flag, void *ustg);
People::People() {

}

People::~People() {

}

#pragma region Mopi
Mat srcPeople, dstPeople;
int mopiStrength;
void People::MoPi() {
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	srcPeople = imread(fileName);
	dstPeople = Mat::zeros(srcPeople.size(), srcPeople.type());

}

void People::mopiStrengthen(int value) {
	mopiStrength = value;
	int value2 = 5;
	cout << mopiStrength << endl;
	int dx = mopiStrength * 5;    //˫���˲�����֮һ
	double fc = mopiStrength*12.5; //˫���˲�����֮һ
	int p = 50;//͸����
	Mat temp1, temp2, temp3, temp4;
	//˫���˲�
	//bilateralFilter(srcPeople, temp1, dx, fc, fc);
	bilateralFilter(srcPeople, temp1, 8, dx, fc, 4);
	temp2 = (temp1 - srcPeople + 128);
	//��˹ģ��
	GaussianBlur(temp2, temp3, Size(2 * value2 - 1, 2 * value2 - 1), 0, 0);
	temp4 = srcPeople + 2 * temp3 - 255;
	dstPeople = (srcPeople*(100 - p) + temp4*p) / 100;
	imshow("origin", srcPeople);
	imshow("mopi", dstPeople);
}

void People::saveFromMoPi() {
	file->WriteData("D:\\savedPhoto.jpg", dstPeople);
}
#pragma endregion
//����ת������
#pragma region white
double  highlights_add[256], highlights_sub[256];
double  midtones_add[256], midtones_sub[256];
double  shadows_add[256], shadows_sub[256];
//��ʼ��ת������
void initWhite() {
	for (int i = 0; i < 256; i++)
	{
		highlights_add[i] = shadows_sub[255 - i] = (1.075 - 1 / ((double)i / 16.0 + 1));
		midtones_add[i] = midtones_sub[i] = 0.667 * (1 - (((double)i - 127.0) / 127.0)*(((double)i - 127.0) / 127.0));
		shadows_add[i] = highlights_sub[i] = 0.667 * (1 - (((double)i - 127.0) / 127.0)*(((double)i - 127.0) / 127.0));
	}
}
void People::White() {
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	srcPeople = imread(fileName);
	dstPeople = Mat::zeros(srcPeople.size(), srcPeople.type());
	srcPeople.copyTo(dstPeople);
	initWhite();
	//cout << value << endl;
}
//ʵ����غ���
void People::whiteStrengthen(int value) {

	srcPeople.copyTo(dstPeople);
	BalanceColor(dstPeople, value);
	//BalanceColor(dstPeople, 50);
	imshow("MeiBai", dstPeople);
}
int FMax(const int X, const int Y) {
return (X < Y ? Y : X);
}
int FMin(const int X, const int Y) {
	return (Y < X ? Y : X);
}
void BalanceColor(Mat& bitmap, int value)
{
	int red, green, blue;
	unsigned char r_lookup[256], g_lookup[256], b_lookup[256];
	for (int i = 0; i < 256; i++)
	{
		red = i;
		green = i;
		blue = i;
		red += (int)(0.0 * shadows_sub[red] + value * midtones_add[red] + 0.0 * highlights_sub[red]);
		red = FMax(0, FMin(0xFF, red));
		green += (int)(0.0 * shadows_sub[green] + value * midtones_add[green] + 0.0 * highlights_sub[green]);
		green = FMax(0, FMin(0xFF, green));
		blue += (int)(0.0 * shadows_sub[blue] + value * midtones_add[blue] + 0.0 * highlights_sub[blue]);
		blue = FMax(0, FMin(0xFF, blue));
		r_lookup[i] = (unsigned char)red;
		//cout << red << endl;
		g_lookup[i] = (unsigned char)green;
		b_lookup[i] = (unsigned char)blue;
	}
	for (int row = 0; row < bitmap.rows; row++)
	{
		for (int col = 0; col < bitmap.cols; col++)
		{
			bitmap.at<Vec3b>(row, col)[0] = b_lookup[bitmap.at<Vec3b>(row, col)[0]];
			bitmap.at<Vec3b>(row, col)[1] = g_lookup[bitmap.at<Vec3b>(row, col)[1]];
			bitmap.at<Vec3b>(row, col)[2] = r_lookup[bitmap.at<Vec3b>(row, col)[2]];
		}
	}
}
void People::saveFromWhite() {
	file->WriteData("D:\\savedPhoto.jpg", dstPeople);
}
#pragma endregion

#pragma region Slim
int slimStartX, slimStartY;
int slimStopX, slimStopY;
Mat slimImg;
void People::Slim(int value) {
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	slimImg = imread(fileName);
	namedWindow("slim");
	setMouseCallback("slim", Slim_OnMouse);
	waitKey();
}

void LocalTranslationWarp(Mat &img, int warpX, int warpY, int warpW, int warpH, int directionX, int directionY, double warpCoef)
{
	//RestrictBounds(warpX, warpY, warpW, warpH);

	Mat imgCopy;
	Mat dstCopy;
	copyMakeBorder(img, imgCopy, 0, 1, 0, 1, BORDER_REPLICATE);

	Point center(warpX + (warpW >> 1), warpY + (warpH >> 1));
	double radius = (warpW < warpH) ? (warpW >> 1) : (warpH >> 1);
	radius = radius * radius;

	// ƽ�Ʒ���ʸ��/ģ
	double transVecX = directionX - center.x;
	double transVecY = directionY - center.y;
	double transVecModel = transVecX * transVecX + transVecY * transVecY;

	// ˮƽ/��ֱ����//ӳ���λ����ԭλ��
	double dx = 0, dy = 0, posX = 0.0, posY = 0.0, posU = 0.0, posV = 0.0;
	// �㵽Բ�ľ���/ƽ�Ʊ���
	double distance = 0.0, ratio = 0.0;
	// ��ֵλ��
	int startU = 0, startV = 0;
	double alpha = 0.0, beta = 0.0;

	int maxRow = warpY + warpH;
	int maxCol = warpX + warpW;
	uchar* pImg = NULL;
	for (int i = warpY; i < maxRow; i++)
	{
		pImg = img.data + img.step * i;
		for (int j = warpX; j < maxCol; j++)
		{
			posX = j;
			posY = i;
			dx = posX - center.x;
			dy = posY - center.y;
			distance = dx * dx + dy * dy;
			if (distance < radius)
			{
				ratio = (radius - distance) / (radius - distance + transVecModel * warpCoef);
				posU = posX - ratio * ratio * transVecX;
				posV = posY - ratio * ratio * transVecY;

				startU = (int)posU;
				startV = (int)posV;
				alpha = posU - startU;
				beta = posV - startV;
				//BilinearInter(imgCopy, startU, startV, alpha, beta, pImg[3 * j], pImg[3 * j + 1], pImg[3 * j + 2]);
				imgCopy= BilinearInter(imgCopy);
			}
		}
	}
	imgCopy.copyTo(slimImg);
	imshow("slim", slimImg);
	waitKey();
}
Mat BilinearInter(Mat& image) {
	/*Vec3b p;
	p[0] = alpha * beta * blue;
	p[1] = alpha * beta*green;
	p[2] = alpha * beta*red;
	img.at<Vec3b>(startU, startV) = p;*/

	int rows = image.rows, cols = image.cols, step = image.step;
	
	//����Ŀ��ͼ��
	cv::Mat result = cv::Mat(cv::Size(150, 150), image.type(), cv::Scalar::all(0));
	int resultrows = result.rows, resultcols = result.cols, resultstep = result.step;
	IplImage src = image, dst = result;
	for (int i = 0; i < resultrows - 1; i++)
	{
		for (int j = 0; j < resultcols - 1; j++)
		{
			float srcx = (j + 0.5)*((float)cols) / resultcols - 0.5;
			float srcy = (i + 0.5)*((float)rows) / resultrows - 0.5;
			int xx = (int)srcx, yy = (int)srcy;
			//3ͨ���������Ȩֵ1
			float t1x = ((uchar*)(src.imageData + step * yy))[xx * 3] * (1 - (srcx - xx)) +
				((uchar*)(src.imageData + step * yy))[(xx + 1) * 3] * (srcx - xx);
			float t1y = ((uchar*)(src.imageData + step * yy))[xx * 3 + 1] * (1 - (srcx - xx)) +
				((uchar*)(src.imageData + step * yy))[(xx + 1) * 3 + 1] * (srcx - xx);
			float t1z = ((uchar*)(src.imageData + step * yy))[xx * 3 + 2] * (1 - (srcx - xx)) +
				((uchar*)(src.imageData + step * yy))[(xx + 1) * 3 + 2] * (srcx - xx);
			//3ͨ���������Ȩֵ2
			float t2x = ((uchar*)(src.imageData + step * (yy + 1)))[xx * 3] * (1 - (srcx - xx)) +
				((uchar*)(src.imageData + step * (yy + 1)))[(xx + 1) * 3] * (srcx - xx);
			float t2y = ((uchar*)(src.imageData + step * (yy + 1)))[xx * 3 + 1] * (1 - (srcx - xx)) +
				((uchar*)(src.imageData + step * (yy + 1)))[(xx + 1) * 3 + 1] * (srcx - xx);
			float t2z = ((uchar*)(src.imageData + step * (yy + 1)))[xx * 3 + 2] * (1 - (srcx - xx)) +
				((uchar*)(src.imageData + step * (yy + 1)))[(xx + 1) * 3 + 2] * (srcx - xx);
			//���ݹ�ʽ���Ŀ��ͼ���Ȩ
			((uchar*)(dst.imageData + resultstep * i))[j * 3] = t1x * (1 - (srcy - yy)) + t2x * (srcy - yy);
			((uchar*)(dst.imageData + resultstep * i))[j * 3 + 1] = t1y * (1 - (srcy - yy)) + t2y * (srcy - yy);
			((uchar*)(dst.imageData + resultstep * i))[j * 3 + 2] = t1z * (1 - (srcy - yy)) + t2z * (srcy - yy);
		}
		//�в���
		((uchar*)(dst.imageData + resultstep * i))[(resultcols - 1) * 3] = ((uchar*)(dst.imageData + resultstep * i))[(resultcols - 2) * 3];
		((uchar*)(dst.imageData + resultstep * i))[(resultcols - 1) * 3 + 1] = ((uchar*)(dst.imageData + resultstep * i))[(resultcols - 2) * 3 + 1];
		((uchar*)(dst.imageData + resultstep * i))[(resultcols - 1) * 3 + 2] = ((uchar*)(dst.imageData + resultstep * i))[(resultcols - 2) * 3 + 2];
	}
	//�в���
	for (int i = 0; i < resultcols * 3; i++)
		((uchar*)(dst.imageData + resultstep * (resultrows - 1)))[i] = ((uchar*)(dst.imageData + resultstep * (resultrows - 2)))[i];
	cv::Mat result1 = cv::cvarrToMat(&dst, true);
	return result1;

}

void Slim_OnMouse(int event, int x, int y, int flag, void *ustg)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		slimStartX = x;
		slimStartY = y;
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		slimStopX = x;
		slimStopY = y;
		cout << "slim......" << endl;
		LocalTranslationWarp(slimImg, slimStartX, slimStartY, 5, 5, slimStopX, slimStopY, 100);
	}


}
#pragma endregion

#pragma region RemoveZhou_Hei
Point ptL, ptR; //��껭�����ο�������յ�
Mat imageSource, imageSourceCopy;
Mat ROIRemove; //ԭͼ��Ҫ�޸������ROI



//���Ȧ������
int People::Remove()
{
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	imageSource = imread(fileName);
	if (!imageSource.data)
	{
		return -1;
	}
	imshow("origin", imageSource);

	char key;
	while (1) {
		setMouseCallback("origin", Remove_OnMouse);
		waitKey();
		key = waitKey(1);
		if (key == 27)
			break;
	}

}
void Remove_OnMouse(int event, int x, int y, int flag, void *ustg)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		ptL = Point(x, y);
		ptR = Point(x, y);
	}
	if (flag == CV_EVENT_FLAG_LBUTTON)
	{
		ptR = Point(x, y);
		imageSourceCopy = imageSource.clone();
		rectangle(imageSourceCopy, ptL, ptR, Scalar(255, 0, 0));
		imshow("origin", imageSourceCopy);
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		if (ptL != ptR)
		{
			ROIRemove = imageSource(Rect(ptL, ptR));
			//imshow("ROI", ROIRemove);
			waitKey();
		}
	}
	//��������Ҽ���ʼͼ���޸�
	if (event == CV_EVENT_RBUTTONDOWN)
	{
		imageSourceCopy = Mat(imageSource.size(), CV_8UC1, Scalar::all(0));
		Mat imageMask = imageSourceCopy(Rect(ptL, ptR));
		//����һ����ROI��Сһ����ֵȫΪ1������
		Mat imageMaskCopy = Mat(imageMask.size(), CV_8UC1, Scalar::all(1));
		imageMaskCopy.copyTo(imageMask);
		inpaint(imageSource, imageSourceCopy, imageSource, 9, INPAINT_TELEA);  //ͼ���޸�
		//imshow("Mask", imageSourceCopy);
		imshow("origin", imageSource);
	}
}

void People::saveFromRemove() {
	file->WriteData("D:\\savedPhoto.jpg", imageSource);
}
#pragma endregion

#pragma region Heighten

Point HptL, HptR; //��껭�����ο�������յ�
Mat imageHeight, imageHeightCopy;
Mat ROI; //ԭͼ��Ҫ�޸������ROI
int ini_x, ini_y;
double Heightensize = 0.8;
void Heighten_OnMouse(int event, int x, int y, int flag, void *ustg)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		HptL = Point(x, y);
		ini_x = x;
		ini_y = y;
		HptR = Point(x, y);
		//printf("%d %d", x,y);
	}
	if (flag == CV_EVENT_FLAG_LBUTTON)
	{
		HptR = Point(x, y);
		imageHeightCopy = imageHeight.clone();
		rectangle(imageHeightCopy, HptL, HptR, Scalar(255, 0, 0));
		imshow("origin", imageHeightCopy);
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		if (HptL != HptR)
		{
			ROI = imageHeight(Rect(HptL, HptR));
			imshow("ROI", ROI);
			waitKey();
		}
	}
	//��������Ҽ���ʼͼ���޸�
	if (event == CV_EVENT_RBUTTONDOWN)
	{
		Mat dst, img2, img3, img5;//dstΪ�쳤��roi
								  //���Ϊԭ����0.8-1.5��
		//double size = 0.8;
		resize(ROI, dst, Size(), 1, Heightensize);
		//printf("%d%d",x, y);
		img2 = imageHeight(Range(ini_y, ini_y + dst.rows), Range(ini_x, ini_x + dst.cols));//ѡȡԭʼͼƬ�д�(100,100)��ʼ��dstͬ����С��ROI����
		img3 = dst.clone();//������ͼ���Ƶ�img3�У��˸�����ͬ��������һ����
		img3.copyTo(img2);//��img3ͼ���Ƶ�ԭͼ�����������У���ʱimageSourece����Ӧ��ROI��������Ҳһ�����˱仯

		//imshow("ROI��������ͼ", dst);//ROI����ͼ��
		imshow("Heightened", imageHeight);//��ʾ�ϲ����ͼ��
	}
}
void People::Heighten() {
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	imageHeight = imread(fileName);
	if (!imageHeight.data)
	{
		return;
	}
	imshow("origin", imageHeight);

	
	//	if (key == 27)
		//	break;
	//}
}
void People::heightLength(int value) {
	Heightensize = (value+50)/100.0;
	char key;
	//while (1) {
	setMouseCallback("origin", Heighten_OnMouse);
	waitKey();
	key = waitKey(1);
}

void People::saveFromHeighten() {
	file->WriteData("D::\\savedPhoto.jpg", imageHeight);
}
#pragma endregion

