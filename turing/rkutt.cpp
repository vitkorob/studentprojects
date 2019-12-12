#include "rkutt.h"

RKutt::RKutt()
{
    A = 1;
    B = 5;
    epsilon = 0.00000001;
    x0 = 7.0;
    y0 = 0.8;
    t = 50.0;
    step = 0.01;
}

double RKutt::p(double x2, double y2)
{
    return A + x2 * x2 * y2 - (B + 1) * x2;
}

double RKutt::q(double x2, double y2)
{
    return B * x2 - x2 * x2 * y2;
}

void RKutt::nextValue(double *x, double *y, double h)
{
    double x1 = h * p(*x, *y);
    double y1 = h * q(*x, *y);
    double x2 = h * p(*x + x1 * 0.5, *y + y1 * 0.5);
    double y2 = h * q(*x + x1 * 0.5, *y + y1 * 0.5);
    double x3 = h * p(*x + x2 * 0.5, *y + y2 * 0.5);
    double y3 = h * q(*x + x2 * 0.5, *y + y2 * 0.5);
    double x4 = h * p(*x + x3, *y + y3);
    double y4 = h * q(*x + x3, *y + y3);

    *x += (x1 + 2 * x2 + 2 * x3 + x4) / 6;
    *y += (y1 + 2 * y2 + 2 * y3 + y4) / 6;
}

void RKutt::run()
{
    pnt_t.clear();
    pnt_x.clear();
    pnt_y.clear();

    pnt_t.append(0.0);
    pnt_x.append(x0);
    pnt_y.append(y0);

    double next_t0 = 0.0;
    FILE *data = fopen("turing-data.txt", "w");

    double next_x0;
    double next_y0;

    double next_x1;
    double next_y1;

    double next_x2;
    double next_y2;

    double next_x3;
    double next_y3;

    double next_x_tmp = x0;
    double next_y_tmp = y0;

    while(next_t0 < t)
    {
        next_x0 = next_x1 = next_x2 = next_x_tmp;
        next_y0 = next_y1 = next_y2 = next_y_tmp;

        nextValue(&next_x0, &next_y0, step);
        nextValue(&next_x1, &next_y1, 2 * step);

        next_x3 = next_x0;
        next_y3 = next_y0;

        nextValue(&next_x3, &next_y3, step);

        nextValue(&next_x2, &next_y2, 0.5 * step);
        nextValue(&next_x2, &next_y2, 0.5 * step);

        if(fabs(next_x0 - next_x2) / 15 <= epsilon && fabs(next_x3 - next_x1) / 15 > epsilon)
        {
            pnt_t.append(next_t0 += step);
            pnt_x.append(next_x_tmp = next_x0);
            pnt_y.append(next_y_tmp = next_y0);

            fprintf(data, "%f\t%f\t%f\t%.12f\n", next_t0, next_x0, next_y0, step);
        }
        else if(fabs(next_x3 - next_x1) / 15 <= epsilon)
        {
            step *= 2;
        }
        else
        {
            step *= 0.5;
        }
    }

    fclose(data);
}
