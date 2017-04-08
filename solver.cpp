/*
 *  solver.cpp
 *  lorenz
 *
 *  Created by McK on 16/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>

#include "vector.hpp"

#include "solver.h"

int solver = 3;
char *solverNames[] = { "Euler", "EulerMod", "RungeKutta2", "RungeKutta3", "RungeKutta4", "RKF" };

// Runge Kutta
double error = 10000;
double minh = 0.0001;
double maxh = 0.01;
double dt = minh;
double t = 0.0;
int skip = 2;
int points = 100000;

// Lorenz
double sigma = 10;
double b = 8.0 / 3.0;
double r = 28;

/*
Lorenz::Lorenz(double sigma, double r, double b) {
	params.push_back(sigma);
	params.push_back(r);
	params.push_back(b);
	paramNames.push_back(string("sigma"));
	paramNames.push_back(string("r"));
	paramNames.push_back(string("b"));
}*/


Vector initial(0.0, 1.0, 1.0, 1.0);
Vector currentPoint(initial);

#define x currentPoint[1]
#define y currentPoint[2]
#define z currentPoint[3]


Vector fLorenz(double t, Vector previousPoint) {
    Vector f(4);
    f[1] = - sigma * x + sigma * y;
    f[2] = - x * z + r * x - y;
    f[3] = x * y - b * z;
    return f;
}


Vector fLorenz84(double t, Vector previousPoint) {
	
	double A=sigma;
	double B=b;
	double F=r;
	double G=1;
	
    Vector f(4);
    f[1] = - A * x - pow(y, 2) - pow(z, 2) + A*F;
    f[2] = -y + x*y - B * x * z + G; 
    f[3] = -z + B * x*y + x*z;
    return f;
}


Vector fPickover(double t, Vector previousPoint) {
	
	sigma = 1;
	b = 1.8;
	r = 0.71;
	
	
	double A = sigma;
	double B = b;
	double C = r;
	double D = 1.51;
	
	Vector f(4);
	f[1] = sin ( A*y) - z * cos(B * x);
	f[2] = z * sin (C*x) - cos(D * y);
	f[3] = sin(x);
	return f;
}


double gam = 0.87;
double alphavalue = 1.1;

// http://en.wikipedia.org/wiki/Rabinovich-Fabrikant_equations
Vector fRabinovich(double t, Vector previousPoint) {
	double r = sqrt(x*x + y*y);
	
    Vector f(4);
    f[1] = y*(z-1+x*x)+gam*x;
    f[2] = x*(3*z+1-x*x)+gam*y;
    f[3] = -2*z*(alphavalue+x*y);
    return f;
}


// http://en.wikipedia.org/wiki/R%C3%B6ssler_map
Vector fRossler(double t, Vector previousPoint) {
	double a = 0.1;
	double b = 0.1;
	double c = 14;
	
	double r = sqrt(x*x + y*y);
	
    Vector f(4);
    f[1] = -y-z;
    f[2] = x+a*y;
    f[3] = b+z*(x-c);
    return f;
}

//http://www.scholarpedia.org/article/Duffing_oscillator



Vector fTest(double t, Vector previousPoint) {
	double r = sqrt(x*x + y*y);
	
    Vector f(4);
    f[1] = 1;
    f[2] = sin(t);
    f[3] = cos(t);
    return f;
}

Vector f(double t, Vector previousPoint) {
	return fLorenz(t, previousPoint);
//	return fLorenz84(t, previousPoint);
	//return fPickover(t, previousPoint);
	//return fRabinovich(t,previousPoint);
	//	return fRossler(t, previousPoint);
	//return fTest(t, previousPoint);
}

#undef x
#undef y
#undef z



void euler(double t, Vector &y , double h) {
	y+= h*f(t, y);
}

void eulerMod(double t, Vector &y , double h) {
	Vector k1 = h * f(t, y);
	Vector k2 = h * f(t + 0.5*h, y + 0.5 * k1);
	y+= k2;
}

void RK2Step(double t, Vector & y, double h) {
	Vector k1 = h * f(t, y);
	Vector k2 = h * f(t + 0.5*h, y + 0.5 * k1);
	
	y+= k2;
}


void RK3Step(double t, Vector& y, double h) {
    Vector k1 = h * f(t, y);
    Vector k2 = h * f(t + 0.5*h, y + 0.5 * k1);
    Vector k3 = h * f(t + h, y -k1 + 2* k2);
	
    y += (k1 + 4*k2 + k3) / 6.0;
}


void RK4Step(double t, Vector& y, double h) {
    Vector k1 = h * f(t, y);
    Vector k2 = h * f(t + 0.5*h, y + 0.5 * k1);
    Vector k3 = h * f(t + 0.5*h, y + 0.5 * k2);
    Vector k4 = h * f(t+h, y + k3);
    y += (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
}

void RKF45Step(double t, Vector& y, double &h) {
    Vector k1 = h * f(t, y);
    Vector k2 = h * f(t + 0.25*h, y + 0.25 * k1);
    Vector k3 = h * f(t + (3.0/8.0)*h, y + (3.0/32.0)*k1 + (9.0/32.0) * k2);
    Vector k4 = h * f(t + (12.0/13.0)*h, y + (1932.0/2197.0)*k1 - (7200.0/2197.0)*k2 + (7296.0/2197.0) * k3);
	Vector k5 = h * f(t + h, y + (439.0/216.0)*k1 - 8*k2 + (3680.0/513.0) * k3 - (845.0/4104.0) * k4);
	Vector k6 = h * f(t + 0.5*h, y - (8.0/27.0)*k1 + 2*k2 - (3544.0/2565.0)*k3 + (1859.0/4104.0)*k4 - (11.0/40.0)*k5);
	
	Vector ord4(4);
	Vector ord5(4);
	ord4 = y + (25.0/216.0)*k1 + (1408.0/2565.0)*k3 + (2197/4101)*k4 - (1.0/5.0)*k5;
	ord5 = y + (16.0/135.0)*k1 + (6656.0/12825.0)*k3 + (28561.0/56430.0)*k4 - (9.0/50.0)*k5 + (2.0/55.0) * k6;
	
	y = ord4;
	
	double s = pow( (error * h * 0.5) / (ord5-ord4).abs(), 0.25);
	
	h = clamp(minh, s * h, maxh);
	
	//std::cout << ord4 << " ¡ " << ord5 << " ¡ " << (ord5-ord4).abs() << " s="<< s << " h="<<h<<std::endl;
}

void step(double &t, Vector &y, double &h) {
	if(solver==0) {
		euler(t,y,h);
	} else if(solver==1) {
		eulerMod(t,y,h);
	} else if(solver==2){
		RK2Step(t,y,h);
	} else if(solver==3) {
		RK3Step(t,y,h);
	} else if(solver==4) {
		RK4Step(t,y,h);
	} else {
		RKF45Step(t, y, h);
	}
	t = t+h;
}
