#include "mainInterface.h"
#include "opencv2\opencv.hpp"
#include "qwidget.h"
#include "beauty.h"
#include "people.h"
#include "puzzle.h"
#include "qfiledialog.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

using namespace std;
using namespace cv;

QPixmap MatToQT(Mat picMat);

MainInterface::MainInterface(QWidget *parent):QWidget(parent) {
	ui = new Ui_Form;
	m_beautyClass = new Beauty();
	m_beautyClass->SetMainUI(this);
	m_peopleClass = new People();
	m_puzzleClass = new Puzzle();
	ui->setupUi(this);

	connect(ui->backFromBeauty, SIGNAL(clicked()), this, SLOT(beauty()));
	connect(ui->backEdit, SIGNAL(clicked()), this, SLOT(loadBeauty()));
	connect(ui->backFromEnhance, SIGNAL(clicked()), this, SLOT(loadBeauty()));
	connect(ui->CancelFromMosaic, SIGNAL(clicked()), this, SLOT(loadBeauty()));
	connect(ui->CancelFromTuya, SIGNAL(clicked()), this, SLOT(loadBeauty()));
	connect(ui->backFromBackground, SIGNAL(clicked()), this, SLOT(loadBeauty()));
	connect(ui->CancelFromZenggao, SIGNAL(clicked()), this, SLOT(people()));
	connect(ui->CancelFromRemove, SIGNAL(clicked()), this, SLOT(people()));
	connect(ui->backFromMoPi, SIGNAL(clicked()), this, SLOT(people()));
	connect(ui->backFromWhite, SIGNAL(clicked()), this, SLOT(people()));
	connect(ui->backFromPeople, SIGNAL(clicked()), this, SLOT(backToMain()));
	connect(ui->backFromBeauty2Main, SIGNAL(clicked()), this, SLOT(backToMain()));


	connect(ui->saveFromBackground, SIGNAL(clicked()), m_beautyClass, SLOT(saveFromBackground()));
	connect(ui->saveFromEnhance, SIGNAL(clicked()), m_beautyClass,SLOT(saveFromEnhance()));
	connect(ui->ConfirmFromMosaic, SIGNAL(clicked()), m_beautyClass, SLOT(saveFromMosaic()));
	connect(ui->ConfirmFromTuya, SIGNAL(clicked()), m_beautyClass, SLOT(saveFromTuYa()));
	connect(ui->saveEdit, SIGNAL(clicked()), m_beautyClass, SLOT(saveFromEdit()));
	connect(ui->saveFromMoPi, SIGNAL(clicked()), m_peopleClass, SLOT(saveFromMoPi()));
	connect(ui->saveFromWhite, SIGNAL(clicked()), m_peopleClass, SLOT(saveFromWhite()));
	connect(ui->ConfirmFromRemove, SIGNAL(clicked()), m_peopleClass, SLOT(saveFromRemove()));
	connect(ui->ConfirmFromZenggao, SIGNAL(clicked()), m_peopleClass, SLOT(saveFromHeighten()));



	ui->liangduSlider->setMinimum(-100);
	ui->liangduSlider->setMaximum(100);
	ui->duibiduSlider->setMinimum(-100);
	ui->duibiduSlider->setMaximum(100);
	ui->ruihuaSlider->setMinimum(-100);
	ui->ruihuaSlider->setMaximum(100);
	ui->baoheduSlider->setMinimum(-100);
	ui->baoheduSlider->setMaximum(100);
	ui->guangbanSlider->setMinimum(-100);
	ui->guangbanSlider->setMaximum(100);

	ui->mosaicSlider->setMinimum(-100);
	ui->mosaicSlider->setMaximum(100);

	ui->blueSlider->setMinimum(0);
	ui->blueSlider->setMaximum(255);
	ui->redSlider->setMinimum(0);
	ui->redSlider->setMaximum(255);
	ui->greenSlider->setMinimum(0);
	ui->greenSlider->setMaximum(255);
	ui->tuyaSlider->setMinimum(-100);
	ui->tuyaSlider->setMaximum(100);
	

	ui->mopiSlider->setMinimum(-100);
	ui->mopiSlider->setMaximum(100);

	ui->whiteSlider->setMinimum(-100);
	ui->whiteSlider->setMaximum(100);

	ui->heightenSlider->setMinimum(-100);
	ui->heightenSlider->setMaximum(100);

	
	

	connect(ui->beautyButton, SIGNAL(clicked()), this, SLOT(beauty()));
	connect(ui->puzzleButton, SIGNAL(clicked()), this, SLOT(puzzle()));
	connect(ui->peopleButton, SIGNAL(clicked()), this, SLOT(people()));
	connect(ui->enterBeautyButton, SIGNAL(clicked()), this, SLOT(loadBeauty()));
	connect(ui->enhance, SIGNAL(clicked()), this, SLOT(loadEnhance()));
	connect(ui->edit, SIGNAL(clicked()), this, SLOT(loadEdit()));
	connect(ui->tailor, SIGNAL(clicked()), m_beautyClass, SLOT(tailor()));
	connect(ui->rotate, SIGNAL(clicked()), m_beautyClass, SLOT(rotate()));
	connect(ui->backgroundXuhua, SIGNAL(clicked()), this, SLOT(loadBackground()));
	connect(ui->mosaic, SIGNAL(clicked()), this, SLOT(loadMosaic()));
	
	connect(ui->Handwrite, SIGNAL(clicked()), this, SLOT(loadTuya()));
	connect(ui->tuyaButton, SIGNAL(clicked()), m_beautyClass, SLOT(Handwritting()));
	connect(ui->blueSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(setblue(int)));
	connect(ui->redSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(setred(int)));
	connect(ui->greenSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(setgreen(int)));
	connect(ui->tuyaSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(setthickness(int)));
	connect(ui->yijianhuanyuan, SIGNAL(clicked()), m_beautyClass, SLOT(restore()));

	connect(ui->mosaicbutton, SIGNAL(clicked()), m_beautyClass, SLOT(Mosaic()));
	connect(ui->mosaicSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(PainterMosaic(int)));
	connect(ui->eraser, SIGNAL(clicked()), m_beautyClass, SLOT(eraseMosaic()));

	connect(ui->liangduSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(liangdu(int)));
	connect(ui->duibiduSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(duibidu(int)));
	connect(ui->ruihuaSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(ruihua(int)));
	connect(ui->baoheduSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(baohedu(int)));
	
	connect(ui->guangbanSlider, SIGNAL(valueChanged(int)), m_beautyClass, SLOT(guangban(int)));


	connect(ui->mopi, SIGNAL(clicked()), this, SLOT(loadMoPi()));
	connect(ui->mopiButton, SIGNAL(clicked()), m_peopleClass, SLOT(MoPi()));
	connect(ui->mopiSlider, SIGNAL(valueChanged(int)), m_peopleClass, SLOT(mopiStrengthen(int)));

	connect(ui->white, SIGNAL(clicked()), this, SLOT(loadWhite()));
	connect(ui->meibaiButton, SIGNAL(clicked()), m_peopleClass, SLOT(White()));
	connect(ui->whiteSlider, SIGNAL(valueChanged(int)), m_peopleClass, SLOT(whiteStrengthen(int)));

	connect(ui->removeZhou_Hei, SIGNAL(clicked()), this, SLOT(loadQvZhou_Hei()));
	connect(ui->removeZhou_Hei, SIGNAL(clicked()), m_peopleClass, SLOT(Remove()));

	connect(ui->slim, SIGNAL(clicked()), this,SLOT(loadSlim()));
	connect(ui->slimSlider, SIGNAL(valueChanged(int)), m_peopleClass, SLOT(Slim(int)));

	connect(ui->puzzleButton, SIGNAL(clicked()), m_puzzleClass, SLOT(addPicture()));
	connect(ui->heighten, SIGNAL(clicked()), this, SLOT(loadHeight()));
	connect(ui->heightenSlider, SIGNAL(valueChanged(int)), m_peopleClass, SLOT(heightLength(int)));
	connect(ui->HeightenButton, SIGNAL(clicked()), m_peopleClass, SLOT(Heighten()));
	ui->stackedWidget->setCurrentIndex(0);
	
}
void MainInterface::backToMain() {
	ui->stackedWidget->setCurrentIndex(0);
	ui->stackedWidget->show();
}

void MainInterface::beauty() {
	Mat selectedPhoto;
	ui->stackedWidget->setCurrentIndex(1);
	//ui->stackedWidget->setCurrentIndex(4);
	ui->stackedWidget->show();
	QString fileName = QFileDialog::getOpenFileName
	(this, tr("请选择图片文件"), "C:\\Users\\cy\\Desktop", tr("type(*png *jpg)"));
	selectedPhoto = imread(fileName.toStdString());
	
	file->WritePath("C:\\Users\\cy\\Desktop\\photo.txt", fileName.toStdString());

	//std::string selectedPath;
	//file->LoadPath("C:\\Users\\cy\\Desktop\\photo.txt",selectedPath);
	//cout << selectedPath << endl;
	
	originalpixmap= MatToQT(selectedPhoto);
	int with = ui->BeautySelectLabel->width();
	int height = ui->BeautySelectLabel->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->BeautySelectLabel->setPixmap(fitpixmap);
	cout << "entering beauty......" << endl;

}
void MainInterface::puzzle() {
	ui->stackedWidget->setCurrentIndex(2);
	ui->stackedWidget->show();
}

void MainInterface::people() {
	Mat selectedPhoto;
	ui->stackedWidget->setCurrentIndex(5);
	ui->stackedWidget->show();
	QString fileName = QFileDialog::getOpenFileName
	(this, tr("请选择图片文件"), "C:\\Users\\cy\\Desktop", tr("type(*png *jpg)"));
	selectedPhoto = imread(fileName.toStdString());
	int with = ui->peoplePhoto->width();
	int height = ui->peoplePhoto->height();

	originalpixmap = MatToQT(selectedPhoto);
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	file->WritePath("C:\\Users\\cy\\Desktop\\photo.txt", fileName.toStdString());

	std::string selectedPath;
	file->LoadPath("C:\\Users\\cy\\Desktop\\photo.txt", selectedPath);
	//cout << selectedPath << endl;

	//ui->peoplePhoto->setPixmap(originalpixmap);
	ui->peoplePhoto->setPixmap(fitpixmap);
	cout << "entering beauty......" << endl;
}

void MainInterface::loadBeauty() {
	ui->stackedWidget->setCurrentIndex(4);
	ui->stackedWidget->show();
	int with = ui->beautyPhoto->width();
	int height = ui->beautyPhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->beautyPhoto->setPixmap(fitpixmap);
}

void MainInterface::loadEdit() {
	ui->stackedWidget->setCurrentIndex(6);
	ui->stackedWidget->show();
	int with = ui->editPhoto->width();
	int height = ui->editPhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->editPhoto->setPixmap(fitpixmap);
}

void MainInterface::loadEnhance() {
	ui->stackedWidget->setCurrentIndex(7);
	ui->stackedWidget->show();
	int with = ui->enhancePhoto->width();
	int height = ui->enhancePhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->enhancePhoto->setPixmap(fitpixmap);
}

void MainInterface::loadBackground() {
	ui->stackedWidget->setCurrentIndex(11);
	ui->stackedWidget->show();
	int with = ui->backgroundPhoto->width();
	int height = ui->backgroundPhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->backgroundPhoto->setPixmap(fitpixmap);
}

void MainInterface::loadMosaic() {
	ui->stackedWidget->setCurrentIndex(8);
	ui->stackedWidget->show();
	int with = ui->mosaicPhoto->width();
	int height = ui->mosaicPhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->mosaicPhoto->setPixmap(fitpixmap);
}

void MainInterface::loadTuya() {
	ui->stackedWidget->setCurrentIndex(9);
	ui->stackedWidget->show();
	int with = ui->tuyaPhoto->width();
	int height = ui->tuyaPhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->tuyaPhoto->setPixmap(fitpixmap);
}

void MainInterface::loadMoPi() {
	ui->stackedWidget->setCurrentIndex(12);
	ui->stackedWidget->show();
	int with = ui->mopiPhoto->width();
	int height = ui->mopiPhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->mopiPhoto->setPixmap(fitpixmap);
}

void MainInterface::loadWhite() {
	ui->stackedWidget->setCurrentIndex(13);
	ui->stackedWidget->show();
	int with = ui->whitePhoto->width();
	int height = ui->whitePhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->whitePhoto->setPixmap(fitpixmap);
}

void MainInterface::loadQvZhou_Hei() {
	ui->stackedWidget->setCurrentIndex(16);
	ui->stackedWidget->show();
	int with = ui->qvdouPhoto->width();
	int height = ui->qvdouPhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->qvdouPhoto->setPixmap(fitpixmap);
}

void MainInterface::loadSlim() {
	ui->stackedWidget->setCurrentIndex(14);
	ui->stackedWidget->show();
	int with = ui->slimPhoto->width();
	int height = ui->slimPhoto->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->slimPhoto->setPixmap(fitpixmap);
}

void MainInterface::loadHeight() {
	ui->stackedWidget->setCurrentIndex(17);
	ui->stackedWidget->show();
	int with = ui->picture->width();
	int height = ui->picture->height();
	QPixmap fitpixmap = originalpixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui->picture->setPixmap(fitpixmap);
}

MainInterface::~MainInterface() {

}

// 功能： 将Mat图像 转换为 QT 的图像
//输入： Mat opencv类图像
//输出： QPixmap QT类图像
QPixmap MatToQT(Mat picMat) {

	QImage picQImage;
	QPixmap picQPixmap;

	cvtColor(picMat, picMat, CV_BGR2RGB);//三通道图片需bgr翻转成rgb

	picQImage = QImage((uchar*)picMat.data, picMat.cols, picMat.rows, QImage::Format_RGB888);

	picQPixmap = QPixmap::fromImage(picQImage);
	return picQPixmap;
}




