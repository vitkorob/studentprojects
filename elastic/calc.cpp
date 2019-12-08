#include "calc.h"

calc::calc()
{
    abc = true;

    mass = 0.6;
    koef = 28.0;
    length = 1.0;
    ygol = 2.0;
    coord = 1.0;
    yglvel = 0.0;
    coordvel = 0.5;
    grav = 9.81;
    eps = 0.0000005;
    time = 30.0;
}

double calc::func_dr(double d)
{
    return d;
}

double calc::func_dfi(double f)
{
    return f;
}

double calc::func_dd(double f, double r, double fi)
{
    return f * f * r + (length - r) * koef / mass + grav * cos(fi);
}

double calc::func_df(double r, double d, double f, double fi)
{
    return (-2.0) * d * f / r - sin(fi) * grav / r;
}

void calc::nextValueEuler(double *r, double *d, double *f, double *fi, double hag)
{
    double temp_r = *r + hag * func_dr(*d);
    double temp_d = *d + hag * func_dd(*f, *r, *fi);
    double temp_f = *f + hag * func_df(*r, *d, *f, *fi);
    double temp_fi = *fi + hag * func_dfi(*f);

    *r = temp_r;
    *d = temp_d;
    *f = temp_f;
    *fi = temp_fi;
}

void calc::nextValueRKutt(double *r, double *d, double *f, double *fi, double hag)
{
    double a1 = hag * func_dr(*d);
    double b1 = hag * func_dd(*f, *r, *fi);
    double c1 = hag * func_df(*r, *d, *f, *fi);
    double d1 = hag * func_dfi(*f);
    double a2 = hag * func_dr(*d + b1 * 0.5);
    double b2 = hag * func_dd(*f + c1 * 0.5, *r + a1 * 0.5, *fi + d1 * 0.5);
    double c2 = hag * func_df(*r + a1 * 0.5, *d + b1 * 0.5, *f + c1 * 0.5, *fi + d1 * 0.5);
    double d2 = hag * func_dfi(*f + c1 * 0.5);
    double a3 = hag * func_dr(*d + b2 * 0.5);
    double b3 = hag * func_dd(*f + c2 * 0.5, *r + a2 * 0.5, *fi + d2 * 0.5);
    double c3 = hag * func_df(*r + a2 * 0.5, *d + b2 * 0.5, *f + c2 * 0.5, *fi + d2 * 0.5);
    double d3 = hag * func_dfi(*f + c2 * 0.5);
    double a4 = hag * func_dr(*d + b3);
    double b4 = hag * func_dd(*f + c3, *r + a3, *fi + d3);
    double c4 = hag * func_df(*r + a3, *d + b3, *f + c3, *fi + d3);
    double d4 = hag * func_dfi(*f + c3);

    *r += (a1 + 2 * a2 + 2 * a3 + a4) / 6;
    *d += (b1 + 2 * b2 + 2 * b3 + b4) / 6;
    *f += (c1 + 2 * c2 + 2 * c3 + c4) / 6;
    *fi += (d1 + 2 * d2 + 2 * d3 + d4) / 6;
}

void calc::run()
{
    pnt_r.clear();
    pnt_d.clear();
    pnt_f.clear();
    pnt_fi.clear();
    pnt_t.clear();
    step = 0.001;

    pnt_r.append(coord);
    pnt_d.append(coordvel);
    pnt_f.append(yglvel);
    pnt_fi.append(ygol);
    pnt_t.append(0);

    FILE *data = fopen("./elastic.dat.txt", "w");
    fprintf(data, "# <time>\t<radius>\t<radius vel>\t<angle vel>\t <angle>\t  <step>\n");
    fprintf(data, "%f\t%f\t%f\t%f\t%f\t%f\n", 0.0, coord, coordvel, yglvel, ygol, step);
    FILE *fileMetod = fopen("./elastic.eps.step.txt", "a");

    double next_t0 = 0;

    double next_r0;
    double next_d0;
    double next_f0;
    double next_fi0;

    double next_r1;
    double next_d1;
    double next_f1;
    double next_fi1;

    double next_r2;
    double next_d2;
    double next_f2;
    double next_fi2;

    double next_r3;
    double next_d3;
    double next_f3;
    double next_fi3;

    double next_r_tmp = coord;
    double next_d_tmp = coordvel;
    double next_f_tmp = yglvel;
    double next_fi_tmp = ygol;

    if(abc)
    {
        while(next_t0 < time)
        {
            next_r0 = next_r1 = next_r2 = next_r_tmp;
            next_d0 = next_d1 = next_d2 = next_d_tmp;
            next_f0 = next_f1 = next_f2 = next_f_tmp;
            next_fi0 = next_fi1 = next_fi2 = next_fi_tmp;

            nextValueRKutt(&next_r0, &next_d0, &next_f0, &next_fi0, step);

            nextValueRKutt(&next_r1, &next_d1, &next_f1, &next_fi1, 2.0 * step);

            next_r3 = next_r0;
            next_d3 = next_d0;
            next_f3 = next_f0;
            next_fi3 = next_fi0;

            nextValueRKutt(&next_r3, &next_d3, &next_f3, &next_fi3, step);

            nextValueRKutt(&next_r2, &next_d2, &next_f2, &next_fi2, 0.5 * step);
            nextValueRKutt(&next_r2, &next_d2, &next_f2, &next_fi2, 0.5 * step);

            if((fabs(next_r0 - next_r2) / 15 <= eps && fabs(next_r3 - next_r1) / 15 > eps) || next_d_tmp == 0.0)
            {
                pnt_r.append(next_r_tmp = next_r0);
                pnt_d.append(next_d_tmp = next_d0);
                pnt_f.append(next_f_tmp = next_f0);
                pnt_fi.append(next_fi_tmp = next_fi0);
                pnt_t.append(next_t0 += step);

                fprintf(data, "%f\t%f\t%f\t%f\t%f\t%f\n", next_t0, next_r0, next_d0, next_f0, next_fi0, step);
            }
            else if(fabs(next_r3 - next_r1) / 15 <= eps)
            {
                step *= 2;
            }
            else
            {
                step *= 0.5;
            }
        }

        fprintf(fileMetod, "rkutt\t%.18lf\t%.18lf\n", eps, time / pnt_t.size());
        fclose(fileMetod);
    }
    else
    {
        while(next_t0 < time)
        {
            next_r0 = next_r1 = next_r2 = next_r_tmp;
            next_d0 = next_d1 = next_d2 = next_d_tmp;
            next_f0 = next_f1 = next_f2 = next_f_tmp;
            next_fi0 = next_fi1 = next_fi2 = next_fi_tmp;

            nextValueEuler(&next_r0, &next_d0, &next_f0, &next_fi0, step);

            nextValueEuler(&next_r1, &next_d1, &next_f1, &next_fi1, 2.0 * step);

            next_r3 = next_r0;
            next_d3 = next_d0;
            next_f3 = next_f0;
            next_fi3 = next_fi0;

            nextValueEuler(&next_r3, &next_d3, &next_f3, &next_fi3, step);

            nextValueEuler(&next_r2, &next_d2, &next_f2, &next_fi2, 0.5 * step);
            nextValueEuler(&next_r2, &next_d2, &next_f2, &next_fi2, 0.5 * step);

            if((fabs(next_r0 - next_r2) <= eps && fabs(next_r3 - next_r1) > eps) || next_d_tmp == 0.0)
            {
                pnt_r.append(next_r_tmp = next_r0);
                pnt_d.append(next_d_tmp = next_d0);
                pnt_f.append(next_f_tmp = next_f0);
                pnt_fi.append(next_fi_tmp = next_fi0);
                pnt_t.append(next_t0 += step);

                fprintf(data, "%f\t%f\t%f\t%f\t%f\t%f\n", next_t0, next_r0, next_d0, next_f0, next_fi0, step);
            }
            else if(fabs(next_r3 - next_r1) <= eps)
            {
                step *= 2;
            }
            else
            {
                step *= 0.5;
            }
        }

        fprintf(fileMetod, "euler\t%.18lf\t%.18lf\n", eps, time / pnt_t.size());
        fclose(fileMetod);
    }

    fclose(data);
}
