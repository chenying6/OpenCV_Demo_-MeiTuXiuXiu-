#include "mainInterface.h"
#include <qapplication.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	cout << "��ͼ����V1.0" << endl;
	MainInterface mainInterface;
	mainInterface.show();
	return app.exec();
}