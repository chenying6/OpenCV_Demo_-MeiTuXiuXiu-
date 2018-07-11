#include "puzzle.h"
#include "opencv2\opencv.hpp"
using namespace std;
using namespace cv;

Puzzle::Puzzle() {

}
#define pic_num 4

Mat src, imageROI[pic_num];
Mat srcROI[4];
int srcwidth = 0, srcheight = 0;
int src2_width, src2_height;
char pic_name[pic_num][20];
char bg[200] = "E:\\lena.png";
int picaddr[pic_num][4] = { { 2, 49, 2, 49 },{ 51, 98, 2, 24 },{ 51, 98, 26, 49 },{ 2, 98, 51, 98 } };

void MyResize(Mat& mat1, Mat& mat2, int width, int height) {
	imshow("test",mat1);
	IplImage pI_1 = mat1;
	cout << "6..." << endl;
	cout << "7..." << endl;
	mat2 = cv::Mat(height, width, CV_8UC3, 1);
	IplImage pI_2 = mat2;
	cout << "8..." << endl;
	cvResize(&pI_1, &pI_2, 1);
	cout << "9..." << endl;

}

void Puzzle::init() {
	int i, j;
	memcpy(pic_name[0], "E\\M_2Dmarker1.jpg", sizeof("E\\M_2Dmarker1.jpg"));
	memcpy(pic_name[1], "E\\2d_low_resolution_image.jpg", sizeof("E\\2d_low_resolution_image.jpg"));
	memcpy(pic_name[2], "E:\\lenablue.jpg", sizeof("E:\\lenablue.jpg"));
	memcpy(pic_name[3], "E:\\lena.png", sizeof("E:\\lena.png"));
	cout << pic_name[0] << endl;
	//src = imread(bg);
	std::string fileName;
	file->LoadPath("D:\\photo.txt", fileName);
	src = imread(fileName);
	imshow("src...", src);
	//waitKey(1);
	srcwidth = src.cols;
	srcheight = src.rows;
	cout << "1..." << endl;
	for (i = 0; i< pic_num; i++) {
		for (j = 0; j<4; j++) {
			if (j<2) {
				picaddr[i][j] = picaddr[i][j] * srcwidth / 100;
			}
			else {
				picaddr[i][j] = picaddr[i][j] * srcheight / 100;
			}
		}
		srcROI[i] = imread(pic_name[i], 1);
		imshow("imgROI", srcROI[i]);
		waitKey();
	}
	cout << "2..." << endl;
}

void roiadd(void) {
	int i;
	Mat tmp;
	for (i = 0; i< pic_num; i++) {
		cout << "3..." << endl;
		Rect rect(picaddr[i][0], picaddr[i][2], picaddr[i][1] - picaddr[i][0], picaddr[i][3] - picaddr[i][2]);
		imageROI[i] = src(rect);
		//imshow("imgROI", srcROI[i]);
		cout << "4..." << endl;
		MyResize(srcROI[i], tmp, picaddr[i][1] - picaddr[i][0], picaddr[i][3] - picaddr[i][2]);
		cout << "5..." << endl;
		
		addWeighted(imageROI[i], 0, tmp, 1, 0.0, imageROI[i]);
	
	}
	
	imshow("src", src);
}

void Puzzle::addPicture() {
	char c;
	int i;
	init();
	//roiadd();
	waitKey();
}
void Puzzle::chooseTemplate() {

}
Puzzle::~Puzzle() {

}

