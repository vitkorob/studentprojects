#include <QApplication>
#include "genwnd.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GenWnd qwe;
    qwe.show();
    return app.exec();
}
