#include <algorithm>
#include <cmath>
#include <cstdarg>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "vector.hpp"

Vector::Vector(int dim) {
    v = new double [this->dim = dim];
    for (int i = 0; i < dim; i++) v[i] = 0;
}

Vector::Vector(double v0, double v1) {
    v = new double [dim = 2];
    v[0] = v0;
    v[1] = v1;
}

Vector::Vector(double v0, double v1, double v2) {
    v = new double [dim = 3];
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
}
	
Vector::Vector(double v0, double v1, double v2, double v3) {
	v = new double [dim = 4];
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
}

Vector::Vector(const Vector& dv) {
    v = new double [dim = dv.dim];
    for (int i = 0; i < dim; i++) v[i] = dv.v[i];
}

void Vector::resize(const int dimension) {
    double *old = new double[dim];
    for (int i = 0; i < dim; i++) old[i] = v[i];
    delete [] v;
    v = new double [dimension];
    for (int i = 0; i < dimension; i++)
        v[i] = i < dim ? old[i] : 0;
    dim = dimension;
    delete [] old;
}

void Vector::push_back(const double d) {
    resize(++dim);
    v[dim-1] = d;
}

void Vector::set(const double d0, ...) {
    va_list args;
    v[0] = d0;
    va_start(args, d0);
    for (int i = 1; i < dim; i++)
        v[i] = va_arg(args, double);
    va_end(args);
}

Vector& Vector::operator = (const Vector& dv) {
    if (this != &dv) {
        if (dim != dv.dim) {
            delete [] v;
            v = new double [dim = dv.dim];
        }
        for (int i = 0; i < dim; i++) v[i] = dv[i];
    }
    return *this;
}

Vector& Vector::operator += (const Vector& dv) {
    for (int i = 0; i < dim; i++) v[i] += dv[i];
    return *this;
}

Vector& Vector::operator -= (const Vector& dv) {
    for (int i = 0; i < dim; i++) v[i] -= dv[i];
    return *this;
}

Vector& Vector::operator *= (double d) {
    for (int i = 0; i < dim; i++) v[i] *= d;
    return *this;
}

Vector& Vector::operator /= (double d) {
    for (int i = 0; i < dim; i++) v[i] /= d;
    return *this;
}

Vector operator - (const Vector& dv) {
    int dim = dv.dimension();
    Vector temp(dim);
    for (int i = 0; i < dim; i++) temp[i] = -dv[i];
    return temp;
}

Vector operator * (const Vector& dv, double d) {
    int dim = dv.dimension();
    Vector temp(dim);
    for (int i = 0; i < dim; i++) temp[i] = dv[i] * d;
    return temp;
}

Vector operator * (double d, const Vector& dv) {
    int dim = dv.dimension();
    Vector temp(dim);
    for (int i = 0; i < dim; i++) temp[i] = dv[i] * d;
    return temp;
}

Vector operator / (const Vector& dv, double d) {
    int dim = dv.dimension();
    Vector temp(dim);
    for (int i = 0; i < dim; i++) temp[i] = dv[i] / d;
    return temp;
}

Vector operator + (const Vector& v1, const Vector& v2) {
    int dim = v1.dimension();
    Vector temp(dim);
    for (int i = 0; i < dim; i++) temp[i] = v1[i] + v2[i];
    return temp;
}

Vector operator - (const Vector& v1, const Vector& v2) {
    int dim = v1.dimension();
    Vector temp(dim);
    for (int i = 0; i < dim; i++) temp[i] = v1[i] - v2[i];
    return temp;
}

double Vector::abs() {
    return std::sqrt(norm());
}

double Vector::norm() {
    double sum = 0;
    for (int i = 0; i < dim; i++) sum += v[i] * v[i];
    return sum;
}

double Vector::dot(const Vector& dv) {
    double sum = 0;
    for (int i = 0; i < dim; i++) sum += v[i] * dv[i];
    return sum;
}

std::ostream& operator<<(std::ostream& os, const Vector& dv) {
    for (int i = 0; i < dv.dim; i++) {
        os << dv.v[i] << '\t';
    }
    return os;
}

double dot(const Vector& v1, const Vector& v2) {
    double sum = 0;
    for (int i = 0; i < v1.size(); i++)
        sum += v1[i] * v2[i];
    return sum;
}

