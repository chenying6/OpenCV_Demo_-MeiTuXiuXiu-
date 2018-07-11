#ifndef MainInterface_H
#define MainInterface_H
#include "ui_beauty.h"
#include "file.h"
#include "beauty.h"
#include "people.h"
#include "puzzle.h"

namespace ui {
	class MainInterface:public Ui_Form{};
}


class MainInterface : public QWidget {
	Q_OBJECT
public:
	MainInterface(QWidget *parent = 0);
	virtual ~MainInterface();

private slots:

void backToMain();
	void beauty();
	void puzzle();
	void people();

	void loadBeauty();
	void loadEdit();
	void loadEnhance();
	void loadBackground();
	void loadMosaic();
	void loadTuya();

	void loadMoPi();
	void loadWhite();

	void loadQvZhou_Hei();

	void loadSlim();
	void loadHeight();
public:
	Ui_Form *ui; 
	QPixmap originalpixmap;
	File *file;

private:
	Beauty *m_beautyClass;
	People *m_peopleClass;
	Puzzle *m_puzzleClass;
};

#endif // !MainInterface_H
