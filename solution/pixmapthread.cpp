#include "pixmapthread.h"

void pixmapThread::run()
{
    pixmap = new QImage(QSize(w, h), QImage::Format_RGB32);
    pixmap->fill(0xFFFFFF);

    QPainter pixPainter(pixmap);
    pixPainter.setRenderHint(QPainter::Antialiasing);

    Painter::drawGraph(&pixPainter, 0x0000FF, masZ, masX, numMas, 30, 30, w - 60, (h - 90)>>1, 0, "T", "X");
    Painter::drawGraph(&pixPainter, 0xFF0000, masX, masY, numMas, 30, 60 + ((h - 90)>>1), w - 60, (h - 90)>>1, 0, "X", "V");
}
