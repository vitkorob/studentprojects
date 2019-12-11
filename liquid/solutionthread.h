#ifndef SOLUTIONTHREAD_H
#define SOLUTIONTHREAD_H

#include <QThread>
#include <QVector>
#include <math.h>
#include <stdio.h>

class SolutionThread : public QThread
{
public:
    SolutionThread();

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
    double dV(double x, double v);
    double dX(double v);

    void nextRKutt(double *x, double *v, double step);
    void nextEuler(double *x, double *v, double step);

protected:
    void run();
};

#endif // SOLUTIONTHREAD_H
