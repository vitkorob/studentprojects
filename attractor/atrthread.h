#ifndef ATRTHREAD_H
#define ATRTHREAD_H

#include <QThread>
#include <QVector>
#include <math.h>
#include <stdio.h>

class AtrThread : public QThread
{

public:
    AtrThread();

    bool method_euler;
    float sig;
    float r;
    float b;
    float err;
    float x0;
    float y0;
    float z0;
    float t0;

    QVector<float> pnt_t;
    QVector<float> pnt_x;
    QVector<float> pnt_y;
    QVector<float> pnt_z;

private:
    double dX(double x, double y);
    double dY(double x, double y, double z);
    double dZ(double x, double y, double z);

    void nextEuler(double *x, double *y, double *z, double step);
    void nextRKutt(double *x, double *y, double *z, double step);

protected:
    void run();

};

#endif // ATRTHREAD_H
