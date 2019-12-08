#include "nrthread.h"

NRThread::NRThread()
{
    n0 = 1.0;
    n1 = 1.3;
    angle = 0.0;
    r = 400.0;
    d = 5.0;
    lamda = 0.630;
    rays = 800000;
    pix = 400;
}

void NRThread::run()
{
    uxPlane plane_up(uxVec3(0, 0, d), uxVec3_zero);
    uxPlane plane_down(uxVec3_zero, uxVec3(0, 0, d));
    uxPlane plane_faza(uxVec3(0, 0, 2 * r), uxVec3_zero);
    uxSphere sphere(uxVec3(0, 0, r), r);

    uxMat33 rot;
    uxLine tmp_line;
    uxVec3 tmp_v1, tmp_v2;

    rot.set(uxVec3(-1, 0, 0), angle);
    plane_faza.n = rot * plane_faza.n;

    srand(time(NULL));
    double x0, y0, fi;
    double aperture = sqrt(2 * r * d - d * d);
    double b = 2.0 * aperture / (double)pix;

    QVector<double> a[pix][pix];
    pixmap = new QImage(pix, pix, QImage::Format_RGB32);
    pixmap->fill(0xc0c0c0);

    for(long i = 0; i < rays; i++)
    {
        uxVec3 v0, v1, v2, v3;

        do
        {
            x0 = aperture * 2.0 * rand() / (double)RAND_MAX - aperture;
            y0 = aperture * 2.0 * rand() / (double)RAND_MAX - aperture;
        }
        while(hypot(x0, y0) > aperture);

        v0.set(x0, y0, d);
        fi = asin(n0 * sin(angle) / n1);

        rot.set(uxVec3(-1, 0, 0), fi);
        tmp_v1 = rot * (-plane_up.n);
        tmp_line.a = v0;
        tmp_line.b = v0 + tmp_v1;

        tmp_line.crossSphere(sphere, &tmp_v1, &tmp_v2);

        if(tmp_v1.z <= d) v1 = tmp_v1;
        else v1 = tmp_v2;

        tmp_v1 = v1 - sphere.o;
        tmp_v1.setNormalize();

        fi = acos((tmp_v1 * (v1 - v0)) / (v1 - v0).getLength());
        fi = asin(n1 * sin(fi) / n0);

        rot.set(tmp_v1.crossProduct(v1 - v0), fi);
        tmp_v1 = rot * tmp_v1;
        tmp_line.a = v1;
        tmp_line.b = v1 + tmp_v1;

        tmp_line.crossPlane(plane_down, &v2);

        tmp_v1.set(0, 0, -1);
        fi = acos((tmp_v1 * (v2 - v1)) / (v2 - v1).getLength());
        rot.set(tmp_v1.crossProduct(v2 - v1), -fi);
        tmp_v1 = -(rot * tmp_v1);
        tmp_line.a = v2;
        tmp_line.b = v2 + tmp_v1;

        if(tmp_line.crossSphere(sphere, &tmp_v1, &tmp_v2))
        {
            if(tmp_v1.z <= d) v3 = tmp_v1;
            else if(tmp_v2.z <= d) v3 = tmp_v2;
            else continue;
        }
        else continue;

        fi = plane_faza.getDistance(v0) * n0 / lamda;
        fi += (v1 - v0).getLength() * n1 / lamda;

        int x12 = floor((v1.x + aperture) / b);
        int y12 = floor((v1.y + aperture) / b);

        a[x12][y12].append(fi -= floor(fi));

        fi += (v2 - v1).getLength() * n0 / lamda;
        fi += (v3 - v2).getLength() * n0 / lamda;
        if(n1 > n0) fi += 0.5;

        x12 = floor((v3.x + aperture) / b);
        y12 = floor((v3.y + aperture) / b);

        a[x12][y12].append(fi -= floor(fi));
    }
/*    //    < (E1 + E2 + E3 + ... + En)^2 >

    for(int l = 0; l < pix; l++)
        for(int j = 0; j < pix; j++)
        {
            double r = 0;
            if(!a[l][j].size()) continue;

            for(int e = 0; e < a[l][j].size(); e++)
            {
                r += cos(a[l][j].at(e) * 2 * 3.14159265);
            }

            r /= a[l][j].size();
            r *= r;

            unsigned long i = 255 * r;
            unsigned long t = (i<<8) + (i<<16) + i;
            pixmap->setPixel(QPoint(l, j), t);
        }
*/

    //    I = I1 + I2 + ... + In + 2 * sqrt(I1 * I2) * cos(df) + ...
    int i = 0;
    for(int l = 0; l < pix; l++)
        for(int j = 0; j < pix; j++)
            if(i < a[l][j].size()) i = a[l][j].size();

    for(int l = 0; l < pix; l++)
        for(int j = 0; j < pix; j++)
        {
            double r = 0;
            if(!a[l][j].size()) continue;

            for(int e = 0; e < a[l][j].size(); e++)
                for(int d = 0; d < a[l][j].size(); d++)
                {
                    r += cos((a[l][j].at(e) - a[l][j].at(d)) * 2 * 3.14159265);
                }

            r /= (i*i);

            unsigned long i = 255 * r;
            unsigned long t = (i<<8) + (i<<16) + i;
            pixmap->setPixel(QPoint(l, j), t);
        }

/*    for(int l = 0; l < pix; l++)
        for(int j = 0; j < pix; j++)
        {
            double r = 0;
            if(!a[l][j].size()) continue;

            for(int e = 0; e < a[l][j].size(); e++)
                for(int d = 0; d < a[l][j].size(); d++)
                {
                    r += cos((a[l][j].at(e) - a[l][j].at(d)) * 2 * 3.14159265);
                }

            r /= (a[l][j].size() * a[l][j].size());

            unsigned long i = 255 * r;
            unsigned long t = (i<<8) + (i<<16) + i;
            pixmap->setPixel(QPoint(l, j), t);
        }
*/

}
