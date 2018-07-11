#ifndef PEOPLE_H
#define PEOPLE_H

#include "qwidget.h"
#include "file.h"
class People:public QWidget
{
	Q_OBJECT
public:
	People();
	~People();

private slots:
	void MoPi();
	void White();
	void Slim(int value);
	int Remove();
	void Heighten();
	void mopiStrengthen(int value);
	void whiteStrengthen(int value);
	void heightLength(int value);

	void saveFromMoPi();
	void saveFromWhite();
	void saveFromHeighten();
	void saveFromRemove();
private:
	File* file;
};


#endif // !

