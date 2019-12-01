#include <QApplication>
#include "pndmain.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     PndMain asd;
     asd.show();
     return app.exec();
 }
