/*************************************************
Copyright:CY
Author:Ying Chen
Date:2018-07-11
Description:main
**************************************************/

#include "mainInterface.h"
#include <qapplication.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	cout << "ÃÀÍ¼ÐãÐãV1.0" << endl;
	MainInterface mainInterface;
	mainInterface.show();
	return app.exec();
}