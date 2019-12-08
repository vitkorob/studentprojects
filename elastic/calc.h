#ifndef CALC_H
#define CALC_H

#include <QThread>
#include <QVector>

#include <stdio.h>
#include <math.h>

class calc : public QThread
{
public:
    calc();

    bool abc;

    double mass;
    double koef;
    double length;
    double ygol;
    double coord;
    double yglvel;
    double coordvel;
    double grav;
    double eps;
    double time;

    double step;

    QVector<float> pnt_r;
    QVector<float> pnt_d;
    QVector<float> pnt_f;
    QVector<float> pnt_fi;
    QVector<float> pnt_t;

private:
    double func_dr(double d);
    double func_dfi(double f);
    double func_dd(double f, double r, double fi);
    double func_df(double r, double d, double f, double fi);

    void nextValueRKutt(double *r, double *d, double *f, double *fi, double hag);
    void nextValueEuler(double *r, double *d, double *f, double *fi, double hag);

protected:
    void run();

};

#endif // CALC_H
