#include <QApplication>
#include "shipmain.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     ShipMain asd;
     asd.show();
     return app.exec();
 }
