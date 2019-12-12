#include "solutionthread.h"

solutionThread::solutionThread()
{
    metRKu = 1;
    gam = 0.2;
    g = 9.81;
    L = 2;
    del = 0.9;
    p2 = 4;
    eps = 0.0005;
    x0 = 2;
    v0 = 3;
    t = 30;

    step = 0.01;
}

double solutionThread::p(double y2)
{
    return y2;
}

double solutionThread::q(double x2, double y2, double t2)
{
    return ((-1) * gam * y2) - g * x2 / (L * (1 + del * cos(p2 * t2)));
}

void solutionThread::nextValueEuler(double *x, double *y, double t, double h)
{
    *x += h * p(*y);
    *y += h * q(*x, *y, t);
}

void solutionThread::nextValueRKutt(double *x, double *y, double t, double h)
{
    double x1 = h * p(*y);
    double y1 = h * q(*x, *y, t);
    double x2 = h * p(*y + y1 * 0.5);
    double y2 = h * q(*x + x1 * 0.5, *y + y1 * 0.5, t + h * 0.5);
    double x3 = h * p(*y + y2 * 0.5);
    double y3 = h * q(*x + x2 * 0.5, *y + y2 * 0.5, t + h * 0.5);
    double x4 = h * p(*y + y3);
    double y4 = h * q(*x + x3, *y + y3, t + h);

    *x += (x1 + 2 * x2 + 2 * x3 + x4) / 6;
    *y += (y1 + 2 * y2 + 2 * y3 + y4) / 6;
}

void solutionThread::run()
{
    pnt_t.clear();
    pnt_x.clear();
    pnt_y.clear();

    pnt_t.append(0);
    pnt_x.append(x0);
    pnt_y.append(v0);

    double next_t0 = 0;
    FILE *data = fopen("ivan-data.txt", "w");
    fprintf(data, "%f\t%f\t%f\n", 0.0, x0, v0);

    FILE *fileMetod = fopen("ivan-step.txt", "a");

    double next_x0;
    double next_y0;

    double next_x1;
    double next_y1;

    double next_x2;
    double next_y2;

    double next_x3;
    double next_y3;

    double next_x_tmp = x0;
    double next_y_tmp = v0;

    if(metRKu)
    {
        while(next_t0 < t)
        {
            next_x0 = next_x1 = next_x2 = next_x_tmp;
            next_y0 = next_y1 = next_y2 = next_y_tmp;

            nextValueRKutt(&next_x0, &next_y0, next_t0, step);

            nextValueRKutt(&next_x1, &next_y1, next_t0, 2 * step);

            next_x3 = next_x0;
            next_y3 = next_y0;

            nextValueRKutt(&next_x3, &next_y3, next_t0 + step, step);

            nextValueRKutt(&next_x2, &next_y2, next_t0, 0.5 * step);
            nextValueRKutt(&next_x2, &next_y2, next_t0 + 0.5 * step, 0.5 * step);

            if((fabs(next_x0 - next_x2) / 15 <= eps && fabs(next_x3 - next_x1) / 15 > eps) || next_y_tmp == 0)
            {
                pnt_t.append(next_t0 += step);
                pnt_x.append(next_x_tmp = next_x0);
                pnt_y.append(next_y_tmp = next_y0);

                fprintf(data, "%f\t%f\t%f\n", next_t0, next_x0, next_y0);
            }
            else if(fabs(next_x3 - next_x1) / 15 < eps)
            {
                step *= 2;
            }
            else
            {
                step *= 0.5;
            }
        }

        fprintf(fileMetod, "rkutt\t%.18lf\t%.18lf\n", eps, t / pnt_t.size());
        fclose(fileMetod);
    }
    else
    {
        while(next_t0 < t)
        {
            next_x0 = next_x1 = next_x2 = next_x_tmp;
            next_y0 = next_y1 = next_y2 = next_y_tmp;

            nextValueEuler(&next_x0, &next_y0, next_t0, step);

            nextValueEuler(&next_x1, &next_y1, next_t0, 2 * step);

            next_x3 = next_x0;
            next_y3 = next_y0;

            nextValueEuler(&next_x3, &next_y3, next_t0 + step, step);

            nextValueEuler(&next_x2, &next_y2, next_t0, 0.5 * step);
            nextValueEuler(&next_x2, &next_y2, next_t0 + 0.5 * step, 0.5 * step);

            if((fabs(next_x0 - next_x2) <= eps && fabs(next_x3 - next_x1) > eps) || next_y_tmp == 0)
            {
                pnt_t.append(next_t0 += step);
                pnt_x.append(next_x_tmp = next_x0);
                pnt_y.append(next_y_tmp = next_y0);

                fprintf(data, "%f\t%f\t%f\n", next_t0, next_x0, next_y0);
            }
            else if(fabs(next_x3 - next_x1) < eps)
            {
                step *= 2;
            }
            else
            {
                step *= 0.5;
            }
        }

        fprintf(fileMetod, "euler\t%.18lf\t%.18lf\n", eps, t / pnt_t.size());
        fclose(fileMetod);
    }

    fclose(data);
}
