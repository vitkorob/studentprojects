#include <QApplication>
#include "wndglav.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    WndGlav foo;
    foo.show();
    return app.exec();
}
