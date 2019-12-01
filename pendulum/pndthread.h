#ifndef PNDTHREAD_H
#define PNDTHREAD_H

#include <QThread>
#include <QVector>
#include <math.h>
#include <stdio.h>

class PndThread : public QThread
{
public:
    PndThread();

    bool method_euler;
    float eps;
    float omg;
    float err;
    float x0;
    float v0;
    float t0;

    QVector<float> pnt_t;
    QVector<float> pnt_x;
    QVector<float> pnt_v;

private:
    double dX(double v);
    double dV(double x, double v);

    void nextEuler(double *x, double *v, double step);
    void nextRKutt(double *x, double *v, double step);

protected:
    void run();


};

#endif // PNDTHREAD_H
