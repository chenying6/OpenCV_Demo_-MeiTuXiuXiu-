#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <fstream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

class File {

public:
	void WriteData(std::string fileName, Mat& metaData);
	int WritePath(std::string fileName, std::string selectedPath);
	int LoadData(string fileName, cv::Mat& matData, int matRows = 0, int matCols = 0, int matChns = 0);
	int LoadPath(string fileName, std::string& selectedPath, int matRows = 0, int matCols = 0, int matChns = 0);


};





#endif // !"FILE_H"

