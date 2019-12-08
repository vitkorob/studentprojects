#include <QApplication>
#include "mainwindow.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     MainWindow asd;
     asd.show();
     return app.exec();
 }
