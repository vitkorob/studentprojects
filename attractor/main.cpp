#include <QApplication>
#include "atrmain.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     AtrMain asd;
     asd.show();
     return app.exec();
 }
