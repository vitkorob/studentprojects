#include "solutionthread.h"

SolutionThread::SolutionThread()
{
    method_euler = false;
    eps = 0.02;
    omg = 2;
    err = 0.00005;
    x0 = 35;
    v0 = 0;
    t0 = 50;
}

double SolutionThread::dX(double v)
{
    return v;
}

double SolutionThread::dV(double x, double v)
{
    return (-1) * omg * omg * x - eps * v * fabs(v);
}

void SolutionThread::nextEuler(double *x, double *v, double step)
{
    double a1 = step * dX(*v);
    double b1 = step * dV(*x, *v);

    *x += a1;
    *v += b1;
}

void SolutionThread::nextRKutt(double *x, double *v, double step)
{
    double a1 = step * dX(*v);
    double b1 = step * dV(*x, *v);
    double a2 = step * dX(*v + b1 * 0.5);
    double b2 = step * dV(*x + a1 * 0.5, *v + b1 * 0.5);
    double a3 = step * dX(*v + b2 * 0.5);
    double b3 = step * dV(*x + a2 * 0.5, *v + b2 * 0.5);
    double a4 = step * dX(*v + b3);
    double b4 = step * dV(*x + a3, *v + b3);

    *x += (a1 + 2 * a2 + 2 * a3 + a4) / 6;
    *v += (b1 + 2 * b2 + 2 * b3 + b4) / 6;
}

void SolutionThread::run()
{
    pnt_x.clear();
    pnt_v.clear();
    pnt_t.clear();

    pnt_x.append(x0);
    pnt_v.append(v0);
    pnt_t.append(0.0);

    double step = 0.002;
    double next_t0 = 0.0;

    FILE *fileData = fopen("./liquid.data.txt", "w");

    fprintf(fileData, "# <time>\t <coord>\t <veloc>\t  <step>\n");
    fprintf(fileData, "%f\t%f\t%f\t%f\n", next_t0, x0, v0, step);

    FILE *fileMetod = fopen("./liquid.step.txt", "a");

    double next_x0;
    double next_v0;

    double next_x1;
    double next_v1;

    double next_x2;
    double next_v2;

    double next_x3;
    double next_v3;

    double temp_x = x0;
    double temp_v = v0;

    if(method_euler)
    {
        while(next_t0 < t0)
        {
            next_x0 = next_x1 = next_x2 = temp_x;
            next_v0 = next_v1 = next_v2 = temp_v;

            nextEuler(&next_x0, &next_v0, step);
            nextEuler(&next_x1, &next_v1, 2.0 * step);

            next_x3 = next_x0;
            next_v3 = next_v0;

            nextEuler(&next_x3, &next_v3, step);

            nextEuler(&next_x2, &next_v2, 0.5 * step);
            nextEuler(&next_x2, &next_v2, 0.5 * step);

            if((fabs(next_x0 - next_x2) <= err && fabs(next_x3 - next_x1) > err) || temp_v == 0.0)
            {
                pnt_x.append(temp_x = next_x0);
                pnt_v.append(temp_v = next_v0);
                pnt_t.append(next_t0 += step);

                fprintf(fileData, "%f\t%f\t%f\t%f\n", next_t0, next_x0, next_v0, step);
            }
            else if(fabs(next_x3 - next_x1) <= err)
            {
                step *= 2;
            }
            else
            {
                step *= 0.5;
            }
        }

        fprintf(fileMetod, "euler\t%.16lf\t%.16lf\n", err, t0 / pnt_t.size());
    }
    else
    {
        while(next_t0 < t0)
        {
            next_x0 = next_x1 = next_x2 = temp_x;
            next_v0 = next_v1 = next_v2 = temp_v;

            nextRKutt(&next_x0, &next_v0, step);
            nextRKutt(&next_x1, &next_v1, 2.0 * step);

            next_x3 = next_x0;
            next_v3 = next_v0;

            nextRKutt(&next_x3, &next_v3, step);

            nextRKutt(&next_x2, &next_v2, 0.5 * step);
            nextRKutt(&next_x2, &next_v2, 0.5 * step);

            if((fabs(next_x0 - next_x2) / 15 <= err && fabs(next_x3 - next_x1) / 15 > err) || temp_v == 0.0)
            {
                pnt_x.append(temp_x = next_x0);
                pnt_v.append(temp_v = next_v0);
                pnt_t.append(next_t0 += step);

                fprintf(fileData, "%f\t%f\t%f\t%f\n", next_t0, next_x0, next_v0, step);
            }
            else if(fabs(next_x3 - next_x1) / 15 <= err)
            {
                step *= 2;
            }
            else
            {
                step *= 0.5;
            }
        }

        fprintf(fileMetod, "rkutt\t%.16lf\t%.16lf\n", err, t0 / pnt_t.size());
    }

    fclose(fileMetod);
    fclose(fileData);
}
