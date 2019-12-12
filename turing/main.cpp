#include <QApplication>
#include "wndmain.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    WndMain bar;
    bar.show();
    return app.exec();
}
