#include <QApplication>
#include "genwnd.h"

int main(int argc, char *argv[])  // argc - kolichestvo parametrov komandnoy stroki, argv massiv ukazateley na stroki
{
    QApplication app(argc, argv); // inicializaciya programmi
    GenWnd qwe;         // sozdanie peremennoy steka glavnogo okna
    qwe.show();         // pokazivaem glavnoe okno
    return app.exec();  // prilogenie nachinaet rabotu
}
