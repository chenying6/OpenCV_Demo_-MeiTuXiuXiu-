#ifndef PUZZLE_H
#define PUZZLE_H

#include "qwidget.h"
#include "file.h"

class Puzzle:public QWidget
{
	Q_OBJECT
public:
	Puzzle();
	~Puzzle();

private slots:
	void addPicture();
	void chooseTemplate();
	void init();
private:
	File *file;
};


#endif // !PUZZLE_H
