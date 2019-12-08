#include "uxmath.h"

const uxVec3 uxVec3_zero(0.0, 0.0, 0.0);
const uxMat33 uxMat33_identity(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

double uxVec3::setNormalize()
{
    double length = getLength();
    double invLength = 1.0 / length;

    if(length)
    {
        x *= invLength;
        y *= invLength;
        z *= invLength;
    }

    return length;
}

void uxMat33::set(double alpha, double beta, double gamma)
{
    col1.x = cos(alpha) * cos(beta);
    col1.y = sin(alpha) * cos(beta);
    col1.z = -sin(beta);

    col2.x = cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma);
    col2.y = sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma);
    col2.z = cos(beta) * sin(gamma);

    col3.x = cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma);
    col3.y = sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma);
    col3.z = cos(beta) * cos(gamma);
}

void uxMat33::set(uxVec3 v, double theta)
{
    col1.x = cos(theta) + (1 - cos(theta)) * v.x * v.x;
    col1.y = (1 - cos(theta)) * v.y * v.x + sin(theta) * v.z;
    col1.z = (1 - cos(theta)) * v.z * v.x - sin(theta) * v.y;

    col2.x = (1 - cos(theta)) * v.y * v.x - sin(theta) * v.z;
    col2.y = cos(theta) + (1 - cos(theta)) * v.y * v.y;
    col2.z = (1 - cos(theta)) * v.z * v.y + sin(theta) * v.x;

    col3.x = (1 - cos(theta)) * v.z * v.x + sin(theta) * v.y;
    col3.y = (1 - cos(theta)) * v.z * v.y - sin(theta) * v.x;
    col3.z = cos(theta) + (1 - cos(theta)) * v.z * v.z;
}

void uxMat33::setTranspose()
{
    double tmp;

    tmp = col1.y;
    col1.y = col2.x;
    col2.x = tmp;

    tmp = col1.z;
    col1.z = col3.x;
    col3.x = tmp;

    tmp = col2.z;
    col2.z = col3.y;
    col3.y = tmp;
}

double uxPlane::getDistance(uxVec3 w)
{
    uxVec3 e;

    if(n.z) e.set(0, 0, -p / n.z);
    else if(n.y) e.set(0, -p / n.y, 0);
    else e.set(-p / n.z, 0, 0);

    e = w - e;

    uxVec3 t = n.crossProduct(e);

    if(t.isZero()) return e.getLength();

    t = t.crossProduct(n);
    e = e.crossProduct(t);

    return e.getLength() / t.getLength();
}

bool uxLine::crossPlane(uxPlane &plane, uxVec3 *v)
{
    uxVec3 t = b - a;

    t *= (plane.p - (plane.n * a)) / (plane.n * t);
    *v = a + t;

    return (*v).isValid();
}

bool uxLine::crossSphere(uxSphere &sphere, uxVec3 *v1, uxVec3 *v2)
{
    double dist = getDistance(sphere.o);
    if(dist > sphere.r) return false;

    uxVec3 tmp = sphere.o - a;
    tmp = tmp.crossProduct(getDirect());
    tmp = tmp.crossProduct(getDirect());
    tmp.setNormalize();
    tmp *= dist;

    double s = sqrt(sphere.r * sphere.r - dist * dist);
    *v1 = *v2 = sphere.o + tmp;
    tmp = getDirect();
    tmp.setNormalize();
    tmp *= s;

    *v1 -= tmp;
    *v2 += tmp;

    return (*v1).isValid() && (*v2).isValid();
}
