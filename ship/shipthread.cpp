#include "shipthread.h"

ShipThread::ShipThread()
{
    method_euler = 0;
    b = -0.4;
    err = 0.000000002;
    x0 = 1.5;
    y0 = -1.2;
    t0 = 60.0;
}

double ShipThread::sgn(double w)
{
    return (w > 0.0) ? 1.0 : ((w < 0.0) ? -1.0 : 0.0);
}

double ShipThread::dX(double y)
{
    return y;
}

double ShipThread::dY(double x, double y)
{
    return (-y) - sgn(x + b * y);
}


void ShipThread::nextEuler(double *x, double *y, double step)
{
    double a1 = step * dX(*y);
    double b1 = step * dY(*x, *y);

    *x += a1;
    *y += b1;
}

void ShipThread::nextRKutt(double *x, double *y, double step)
{
    double a1 = step * dX(*y);
    double b1 = step * dY(*x, *y);
    double a2 = step * dX(*y + b1 * 0.5);
    double b2 = step * dY(*x + a1 * 0.5, *y + b1 * 0.5);
    double a3 = step * dX(*y + b2 * 0.5);
    double b3 = step * dY(*x + a2 * 0.5, *y + b2 * 0.5);
    double a4 = step * dX(*y + b3);
    double b4 = step * dY(*x + a3, *y + b3);

    *x += (a1 + 2 * a2 + 2 * a3 + a4) / 6;
    *y += (b1 + 2 * b2 + 2 * b3 + b4) / 6;
}

void ShipThread::run()
{
    pnt_x.clear();
    pnt_y.clear();
    pnt_t.clear();

    pnt_x.append(x0);
    pnt_y.append(y0);
    pnt_t.append(0.0);

    double step = 0.002;
    double next_t0 = 0.0;

    FILE *fileData = fopen("./ship.step.txt", "w");

    fprintf(fileData, "# <time>\t     <x>\t     <y>\t  <step>\n");
    fprintf(fileData, "%f\t%f\t%f\t%f\n", next_t0, x0, y0, step);

    FILE *fileMetod = fopen("./ship.info.txt", "a");

    double next_x0;
    double next_y0;

    double next_x1;
    double next_y1;

    double next_x2;
    double next_y2;

    double next_x3;
    double next_y3;

    double temp_x = x0;
    double temp_y = y0;

    if(method_euler)
    {
        while(next_t0 < t0)
        {
            next_x0 = next_x1 = next_x2 = temp_x;
            next_y0 = next_y1 = next_y2 = temp_y;

            nextEuler(&next_x0, &next_y0, step);
            nextEuler(&next_x1, &next_y1, 2.0 * step);

            next_x3 = next_x0;
            next_y3 = next_y0;

            nextEuler(&next_x3, &next_y3, step);

            nextEuler(&next_x2, &next_y2, 0.5 * step);
            nextEuler(&next_x2, &next_y2, 0.5 * step);

            if((fabs(next_x0 - next_x2) <= err && fabs(next_x3 - next_x1) > err) || temp_y == 0.0)
            {
                pnt_x.append(temp_x = next_x0);
                pnt_y.append(temp_y = next_y0);
                pnt_t.append(next_t0 += step);

                fprintf(fileData, "%f\t%f\t%f\t%f\n", next_t0, next_x0, next_y0, step);
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

        fprintf(fileMetod, "euler\t%.14lf\t%.14lf\n", err, t0 / pnt_t.size());
    }
    else
    {
        while(next_t0 < t0)
        {
            next_x0 = next_x1 = next_x2 = temp_x;
            next_y0 = next_y1 = next_y2 = temp_y;
            
            nextRKutt(&next_x0, &next_y0, step);
            nextRKutt(&next_x1, &next_y1, 2.0 * step);

            next_x3 = next_x0;
            next_y3 = next_y0;

            nextRKutt(&next_x3, &next_y3, step);

            nextRKutt(&next_x2, &next_y2, 0.5 * step);
            nextRKutt(&next_x2, &next_y2, 0.5 * step);

            if((fabs(next_x0 - next_x2) / 15 <= err && fabs(next_x3 - next_x1) / 15 > err) || temp_y == 0.0)
            {
                pnt_x.append(temp_x = next_x0);
                pnt_y.append(temp_y = next_y0);
                pnt_t.append(next_t0 += step);

                fprintf(fileData, "%f\t%f\t%f\t%f\n", next_t0, next_x0, next_y0, step);
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

        fprintf(fileMetod, "rkutt\t%.14lf\t%.14lf\n", err, t0 / pnt_t.size());
    }

    fclose(fileMetod);
    fclose(fileData);
}
