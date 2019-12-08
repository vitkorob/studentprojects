#include <QApplication>
#include "nrmainwnd.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    NRMainWnd qwe;
    qwe.show();
    return app.exec();
}
