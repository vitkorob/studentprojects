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
    float frc;
    float per;
    float tau;
    float err;
    float x0;
    float v0;
    float t0;

    QVector<float> pnt_t;
    QVector<float> pnt_x;
    QVector<float> pnt_v;
    QVector<float> pnt_f;

private:
    double dV(double x, double v, double t);
    double dX(double v);
    double f(double t);

    void nextRKutt(double *x, double *v, double t, double step);
    void nextEuler(double *x, double *v, double t, double step);

protected:
    void run();
};

#endif // SOLUTIONTHREAD_H
