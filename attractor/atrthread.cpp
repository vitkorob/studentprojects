#include "atrthread.h"

AtrThread::AtrThread()
{
    method_euler = 0;
    sig = 10.0;
    r = 28.0;
    b = 8.0 / 3.0;
    err = 0.00000002;
    x0 = 2.0;
    y0 = 3.0;
    z0 = 2.5;
    t0 = 30.0;
}

double AtrThread::dX(double x, double y)
{
    return sig * (y - x);
}

double AtrThread::dY(double x, double y, double z)
{
    return x * (r - z) - y;
}

double AtrThread::dZ(double x, double y, double z)
{
    return x * y - b * z;
}

void AtrThread::nextEuler(double *x, double *y, double *z, double step)
{
    double a1 = step * dX(*x, *y);
    double b1 = step * dY(*x, *y, *z);
    double c1 = step * dZ(*x, *y, *z);

    *x += a1;
    *y += b1;
    *z += c1;
}

void AtrThread::nextRKutt(double *x, double *y, double *z, double step)
{
    double a1 = step * dX(*x, *y);
    double b1 = step * dY(*x, *y, *z);
    double c1 = step * dZ(*x, *y, *z);
    double a2 = step * dX(*x + a1 * 0.5, *y + b1 * 0.5);
    double b2 = step * dY(*x + a1 * 0.5, *y + b1 * 0.5, *z + c1 * 0.5);
    double c2 = step * dZ(*x + a1 * 0.5, *y + b1 * 0.5, *z + c1 * 0.5);
    double a3 = step * dX(*x + a2 * 0.5, *y + b2 * 0.5);
    double b3 = step * dY(*x + a2 * 0.5, *y + b2 * 0.5, *z + c2 * 0.5);
    double c3 = step * dZ(*x + a2 * 0.5, *y + b2 * 0.5, *z + c2 * 0.5);
    double a4 = step * dX(*x + a3, *y + b3);
    double b4 = step * dY(*x + a3, *y + b3, *z + c3);
    double c4 = step * dZ(*x + a3, *y + b3, *z + c3);

    *x += (a1 + 2 * a2 + 2 * a3 + a4) / 6;
    *y += (b1 + 2 * b2 + 2 * b3 + b4) / 6;
    *z += (c1 + 2 * c2 + 2 * c3 + c4) / 6;
}

void AtrThread::run()
{
    pnt_x.clear();
    pnt_y.clear();
    pnt_z.clear();
    pnt_t.clear();

    pnt_x.append(x0);
    pnt_y.append(y0);
    pnt_z.append(z0);
    pnt_t.append(0.0);

    double step = 0.002;
    double next_t0 = 0.0;

    FILE *fileData = fopen("./attractor.step.txt", "w");

    fprintf(fileData, "# <time>\t     <x>\t     <y>\t     <z>\t  <step>\n");
    fprintf(fileData, "%f\t%f\t%f\t%f\t%f\n", next_t0, x0, y0, z0, step);

    FILE *fileMetod = fopen("./attractor.info.txt", "a");

    double next_x0;
    double next_y0;
    double next_z0;

    double next_x1;
    double next_y1;
    double next_z1;

    double next_x2;
    double next_y2;
    double next_z2;

    double next_x3;
    double next_y3;
    double next_z3;

    double temp_x = x0;
    double temp_y = y0;
    double temp_z = z0;

    if(method_euler)
    {
        while(next_t0 < t0)
        {
            next_x0 = next_x1 = next_x2 = temp_x;
            next_y0 = next_y1 = next_y2 = temp_y;
            next_z0 = next_z1 = next_z2 = temp_z;

            nextEuler(&next_x0, &next_y0, &next_z0, step);
            nextEuler(&next_x1, &next_y1, &next_z1, 2.0 * step);

            next_x3 = next_x0;
            next_y3 = next_y0;
            next_z3 = next_z0;

            nextEuler(&next_x3, &next_y3, &next_z3, step);

            nextEuler(&next_x2, &next_y2, &next_z2, 0.5 * step);
            nextEuler(&next_x2, &next_y2, &next_z2, 0.5 * step);

            if((fabs(next_x0 - next_x2) <= err && fabs(next_x3 - next_x1) > err) || sig == 0.0)
            {
                pnt_x.append(temp_x = next_x0);
                pnt_y.append(temp_y = next_y0);
                pnt_z.append(temp_z = next_z0);
                pnt_t.append(next_t0 += step);

                fprintf(fileData, "%f\t%f\t%f\t%f\t%f\n", next_t0, next_x0, next_y0, next_z0, step);
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
            next_z0 = next_z1 = next_z2 = temp_z;

            nextRKutt(&next_x0, &next_y0, &next_z0, step);
            nextRKutt(&next_x1, &next_y1, &next_z1, 2.0 * step);

            next_x3 = next_x0;
            next_y3 = next_y0;
            next_z3 = next_z0;

            nextRKutt(&next_x3, &next_y3, &next_z3, step);

            nextRKutt(&next_x2, &next_y2, &next_z2, 0.5 * step);
            nextRKutt(&next_x2, &next_y2, &next_z2, 0.5 * step);

            if((fabs(next_x0 - next_x2) / 15 <= err && fabs(next_x3 - next_x1) / 15 > err) || sig == 0.0)
            {
                pnt_x.append(temp_x = next_x0);
                pnt_y.append(temp_y = next_y0);
                pnt_z.append(temp_z = next_z0);
                pnt_t.append(next_t0 += step);

                fprintf(fileData, "%f\t%f\t%f\t%f\t%f\n", next_t0, next_x0, next_y0, next_z0, step);
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
