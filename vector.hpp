#ifndef CPL_VECTOR_HPP
#define CPL_VECTOR_HPP

#include <iostream>



class Vector {
  public:

    Vector(                
        int dim = 1        
    );                     

    Vector(                
        double v0,         
        double v1          
    );

    Vector(                
        double v0,         
        double v1,         
        double v2          
    );
	
	Vector(                
		   double v0,         
		   double v1,         
		   double v2,          
		   double v3
		   );

    Vector(                
        const Vector&      
    );

    ~Vector() {            
        delete [] v;       
    }

    int dimension() const  
    { return dim; }        

    int size() const       
    { return dim; }

    void resize(           
        const int          
    );                     

    void push_back(        
        const double       
    );

    void set(              
        const double,      
        ...                
    );                     

    const double           
    operator[](            
        const int i        
    ) const                
    { return v[i]; }       

    double&                
    operator[](            
        const int i        
    ) { return v[i]; }     

    

    Vector& operator = (const Vector&);

    Vector& operator += (const Vector&);

    Vector& operator -= (const Vector&);

    

    Vector& operator *= (double);

    Vector& operator /= (double);

    double abs();          

    double norm();         

    double dot(            
       const Vector&       
    );

    
    friend std::ostream& operator<<(std::ostream& os, const Vector& dv);

  private:
    int dim;               
    double *v;             
};




inline Vector operator + (const Vector& vec) { return vec; }


extern Vector operator - (const Vector&);



extern Vector operator * (const Vector&v, double);

extern Vector operator * (double, const Vector&);

extern Vector operator / (const Vector&, double);



extern Vector operator + (const Vector&, const Vector&);

extern Vector operator - (const Vector&, const Vector&);



extern double dot(const Vector&, const Vector&);

#endif /* CPL_VECTOR_HPP */
