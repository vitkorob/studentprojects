#include <QThread>
#include <QVector>
#include <math.h>
#include <stdio.h>

class Euler : public QThread
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

    explicit Euler();

private:
    double p(double x2, double y2);
    double q(double x2, double y2);
    void nextValue(double *x, double *y, double h);

protected:
    void run();

};
