#ifndef SHIPTHREAD_H
#define SHIPTHREAD_H

#include <QThread>
#include <QVector>
#include <math.h>
#include <stdio.h>

class ShipThread : public QThread
{

public:
    ShipThread();

    bool method_euler;
    float b;
    float err;
    float x0;
    float y0;
    float t0;

    QVector<float> pnt_t;
    QVector<float> pnt_x;
    QVector<float> pnt_y;

private:
    double dX(double y);
    double dY(double x, double y);

    double sgn(double w);

    void nextEuler(double *x, double *y, double step);
    void nextRKutt(double *x, double *y, double step);

protected:
    void run();

};

#endif // SHIPTHREAD_H
