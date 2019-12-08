#ifndef NRTHREAD_H
#define NRTHREAD_H

#include <QThread>
#include <QVector>
#include <QPixmap>
#include <stdlib.h>
#include <time.h>
#include "uxmath.h"

class NRThread : public QThread
{

public:
    QImage *pixmap;
    double n0, n1, angle, r, d, lamda;
    long pix, rays;

    NRThread();

protected:
    void run();

};

#endif // NRTHREAD_H
