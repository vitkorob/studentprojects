#include "pixmapthread.h"

void pixmapThread::run()
{
    pixmap = new QImage(QSize(w, h), QImage::Format_RGB32);
    pixmap->fill(0xFFFFFF);

    QPainter pixPainter(pixmap);
    pixPainter.setRenderHint(QPainter::Antialiasing);

    Painter::drawGraph(&pixPainter, 0xEE0000, masZ, masX, numMas, 30, 30, (w - 90)>>1, (h - 90)>>1, 0, "T", "X");
    Painter::drawGraph(&pixPainter, 0x008833, masZ, masY, numMas, 60 + ((w - 90)>>1), 30, (w - 90)>>1, (h - 90)>>1, 0, "T", "Y");
    Painter::drawGraph(&pixPainter, 0x0033FF, masX, masY, numMas, 30, 60 + ((h - 90)>>1), w - 60, (h - 90)>>1, 0, "X", "Y");
}
