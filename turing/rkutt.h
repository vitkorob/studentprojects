#ifndef RKUTT_H
#define RKUTT_H

#include <QThread>
#include <QVector>
#include <math.h>
#include <stdio.h>

class RKutt : public QThread
{
public:
    double A;
    double B;
    double x0;
    double y0;
    double t;
    double epsilon;
    double step;

    QVector<float> pnt_t;
    QVector<float> pnt_x;
    QVector<float> pnt_y;

    explicit RKutt();

private:
    double p(double x2, double y2);
    double q(double x2, double y2);
    void nextValue(double *x, double *y, double h);

protected:
    void run();

};

#endif  // RKUTT_H
