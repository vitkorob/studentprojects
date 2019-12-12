#ifndef SOLUTIONTHREAD_H
#define SOLUTIONTHREAD_H

#include <QThread>
#include <QVector>
#include <math.h>
#include <stdio.h>

class solutionThread : public QThread
{
public:
    solutionThread();

    bool metRKu;
    float gam;
    float g;
    float L;
    float del;
    float p2;
    float eps;
    float x0;
    float v0;
    float t;

    double step;

    QVector<float> pnt_t;
    QVector<float> pnt_x;
    QVector<float> pnt_y;

private:
    double p(double y2);
    double q(double x2, double y2, double t2);

    void nextValueRKutt(double *x, double *y, double t, double h);
    void nextValueEuler(double *x, double *y, double t, double h);

protected:
    void run();
};

#endif // SOLUTIONTHREAD_H
