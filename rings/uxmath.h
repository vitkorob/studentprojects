#ifndef UXMATH_H
#define UXMATH_H

#include <math.h>

inline bool uxIsValid(double x) { if(x != x) return false; return true;}

struct uxVec3
{
    double x, y, z;

    // Default constructor does nothing (for performance).
    uxVec3() {}

    // Construct using coordinates.
    uxVec3(double x, double y, double z) : x(x), y(y), z(z) {}

    // Set this vector to all zeros.
    void setZero() { x = 0.0; y = 0.0; z = 0.0; }

    // Set this vector to some specified coordinates.
    void set(double x_, double y_, double z_) { x = x_; y = y_; z = z_; }

    // Negate this vector.
    uxVec3 operator - () const { return uxVec3(-x, -y, -z); }

    // Sub vectors.
    uxVec3 operator - (uxVec3 v) { return uxVec3(x - v.x, y - v.y, z - v.z); }

    // Add a vector to this vector.
    void operator += (const uxVec3& v) { x += v.x; y += v.y; z += v.z; }

    // Add a vector to this vector.
    uxVec3 operator + (uxVec3 v) { return uxVec3(x + v.x, y + v.y, z + v.z); }

    // Subtract a vector from this vector.
    void operator -= (const uxVec3& v) { x -= v.x; y -= v.y; z -= v.z; }

    // Multiply this vector by a scalar.
    void operator *= (double a) { x *= a; y *= a; z *= a; }

    // Multiply this vector by a scalar.
    uxVec3 operator * (double a) { return uxVec3(x * a, y * a, z * a); }

    // Dot product
    double operator * (uxVec3 v) { return x * v.x + y * v.y + z * v.z; }

    // Cross product
    uxVec3 crossProduct(uxVec3 v) { return uxVec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

    // Get the length of this vector (the norm).
    double getLength() const { return sqrt(x * x + y * y + z * z); }

    // Get the length squared.
    double getLengthSquared() const { return x * x + y * y + z * z; }

    // Distance to a point.
    double distance(uxVec3 w) { uxVec3 i = *this - w; return i.getLength(); }

    // Convert this vector into a unit vector. Returns the length.
    double setNormalize();

    bool isZero() { return x == 0.0 && y == 0.0 && z == 0.0; }

    // Does this vector contain finite coordinates?
    bool isValid() const { return uxIsValid(x) && uxIsValid(y) && uxIsValid(z); }
};

struct uxMat33
{
    uxVec3 col1, col2, col3;

    // Default constructor does nothing (for performance).
    uxMat33() {}

    // Construct using coordinates.
    uxMat33(double a11, double a12, double a13,
            double a21, double a22, double a23,
            double a31, double a32, double a33)
    {
        col1.set(a11, a21, a31);
        col2.set(a12, a22, a32);
        col3.set(a13, a23, a33);
    }

    // Construct using colums.
    uxMat33(uxVec3 col1, uxVec3 col2, uxVec3 col3) : col1(col1), col2(col2), col3(col3) {}

    // Set this matrix to all zeros.
    void setZero() { col1.setZero(); col2.setZero(); col3.setZero(); }

    // Set this matrix through the Euler angles.
    void set(double alpha, double beta, double gamma);

    // Set this matrix using axis and angle.
    void set(uxVec3 v, double theta);

    // Set transpose.
    void setTranspose();

    // Multiply this matrix by a scolar.
    void operator *= (double a) { col1 *= a; col2 *= a; col3 *= a; }

    // Multiply this matrix by a vector.
    uxVec3 operator * (uxVec3 v)
    {
        uxVec3 _v;

        _v.x = col1.x * v.x + col2.x * v.y + col3.x * v.z;
        _v.y = col1.y * v.x + col2.y * v.y + col3.y * v.z;
        _v.z = col1.z * v.x + col2.z * v.y + col3.z * v.z;

        return _v;
    }

    // Multiply this matrix by a matrix.
    uxMat33 operator * (uxMat33 A)
    {
        uxMat33 _A;

        _A.col1 = *this * A.col1;
        _A.col2 = *this * A.col2;
        _A.col3 = *this * A.col3;

        return _A;
    }

    double getDet()
    {
        return col1.x * col2.y * col3.z + col2.x * col3.y * col1.z + col3.x * col1.y * col2.z -
               col3.x * col2.y * col1.z - col2.x * col1.y * col3.z - col1.x * col3.y * col2.z;
    }
};

struct uxPlane
{
    uxVec3 n;
    double p;

    // Default constructor does nothing (for performance).
    uxPlane() {}

    // Constructor using two vectors.
    uxPlane(uxVec3 first, uxVec3 second)
    {
        n = second - first;
        p = -(n * first);

        double invLength = 1.0 / n.getLength();
        if(p > 0.0) invLength = -invLength;

        n *= invLength;
        p *= invLength;
    }

    double getDistance(uxVec3 w);
};

struct uxSphere
{
    uxVec3 o;
    double r;

    // Default constructor does nothing (for performance).
    uxSphere() {}

    uxSphere(uxVec3 o, double r) : o(o), r(r) {}
};

struct uxLine
{
    uxVec3 a, b;

    // Default constructor does nothing (for performance).
    uxLine() {}

    uxLine(uxVec3& first, uxVec3& second) : a(first), b(second) {}

    // Get the direction vector.
    uxVec3 getDirect() { return b - a; }

    // Distance to a point.
    double getDistance(uxVec3 w)
    {
        uxVec3 p = w - a;
        uxVec3 q = getDirect();

        p = q.crossProduct(p);

        return p.getLength() / q.getLength();
    }

    // Compute point of cross with plane.
    bool crossPlane(uxPlane& plane, uxVec3* v);

    // Compute point of cross with sphere.
    bool crossSphere(uxSphere& sphere, uxVec3* v1, uxVec3* v2);
};

extern const uxVec3 uxVec3_zero;
extern const uxMat33 uxMat33_identity;

#endif // UXMATH_H
