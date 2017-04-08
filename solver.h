/*
 *  solver.h
 *  lorenz
 *
 *  Created by McK on 16/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include <string>

using namespace std;

class DiffFunction
{
protected:
	vector<double> params;
	vector<string> paramNames;
public:
	virtual Vector applyFunction(double t, Vector previousPoint);
	vector<double> &getParams();
	vector<string> &getParamNames();
};

class Lorenz : public DiffFunction {

public:
	Lorenz(double sigma, double r, double b);
	Vector applyFunction(double t, Vector previousPoint);
/*	double getSigma() {
		return params[0];
	}
	double getR() {
		return params[1];
	}
	double getB() {
		return params[2];
	}*/
};

class Solver 
{
public:
	virtual void step(double &t, Vector &y, double &h);	
};



class SolverRungeKutta2 : public Solver {
public:
	void step(double &t, Vector &y, double &h);	
};


#define min(a,b) ((a<b)?(a):(b))
#define max(a,b) ((a>b)?(a):(b))
#define clamp(mn,v,mx) max(min(v,mx),mn)

void step(double &t, Vector &y, double &h);