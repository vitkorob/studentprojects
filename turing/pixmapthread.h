#ifndef PIXMAPTHREAD_H
#define PIXMAPTHREAD_H

#include "painter.h"
#include <QThread>
#include <QImage>

class pixmapThread : public QThread
{
public:
    int w;
    int h;

    float *masX;
    float *masY;
    float *masZ;

    unsigned long numMas;

    QImage *pixmap;

protected:
    void run();

};

#endif  // PIXMAPTHREAD_H
