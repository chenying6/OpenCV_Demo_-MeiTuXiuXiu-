#ifndef BEAUTY_H
#define BEAUTY_H

#include "qwidget.h"
#include "opencv2\opencv.hpp"
#include "ui_beauty.h"
#include "file.h"

class MainInterface;
using namespace cv;
class Beauty : public QWidget {
	Q_OBJECT
public:
	Beauty(QWidget *parent = 0);
	virtual ~Beauty();
	
	void SetMainUI(MainInterface* mainUI) { m_mainUI = mainUI; }

public slots:
	
	void Mosaic();
	void PainterMosaic(int value);
	void eraseMosaic();

	void Handwritting();
	void setblue(int value);
	void setred(int value);
	void setgreen(int value);
	void setthickness(int value);
	void restore();

	void Writeword();

	void tailor();
	void rotate();
	void liangdu(int value);
	void duibidu(int value);
	void ruihua(int value);
	void baohedu(int value);

	void guangban(int value);

	void saveFromMosaic();
	void saveFromTuYa();
	void saveFromEnhance();
	void saveFromEdit();
	void saveFromBackground();

private:
	File *file;
	MainInterface *m_mainUI;

};
#endif // !BEAUTY_H
