#ifndef _HOVISFUNCTION_H_
#define _HOVISFUNCTION_H_

#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/FixedConstant.h"
#include <sstream>
#include <cmath>
#include <string>
#include <stdexcept>
using namespace Genfun;

inline std::string toString(int i) {
  std::ostringstream o;
  o << i;
  return o.str();
}

inline GENFUNCTION select(unsigned int N, 
			  GENFUNCTION h0,
			  GENFUNCTION h1,
			  GENFUNCTION h2,
			  GENFUNCTION h3,
			  GENFUNCTION h4,
			  GENFUNCTION h5) {
  if (N==0) return h0;
  if (N==1) return h1;
  if (N==2) return h2;
  if (N==3) return h3;
  if (N==4) return h4;
  if (N==5) return h5;
  throw std::runtime_error("Error in H0VisFunction.h");
  return h0;
  
}
class HOVisFunction: public VisFunction {
 
public:
 
  enum Type {REAL,IMAGINARY,MAGSQ};
  
  HOVisFunction(int N, Type type):VisFunction(std::string("N=")
					      + toString(N) + std::string(
									  type == REAL      ?   std::string(" (real part)") : 
									  type == IMAGINARY ?   std::string(" (imaginary part)"): std::string(" (mag sq)")))
    {
      
      double 
	n0=1/sqrt(sqrt(M_PI)              * 1),
	n1=1/sqrt(sqrt(M_PI)*2            * 1*1),
	n2=1/sqrt(sqrt(M_PI)*2*2          * 1*1*2),
	n3=1/sqrt(sqrt(M_PI)*2*2*2        * 1*1*2*3),
	n4=1/sqrt(sqrt(M_PI)*2*2*2*2      * 1*1*2*3*4),
	n5=1/sqrt(sqrt(M_PI)*2*2*2*2*2    * 1*1*2*3*4*5);
      
      Variable z;
      Exp exp;
      Sin sin;
      Cos cos;
      
	GENFUNCTION
	  h0=n0*exp(-z*z/2),
	  h1=n1*exp(-z*z/2)*2*z,
	  h2=n2*exp(-z*z/2)*(4*z*z -2),
	  h3=n3*exp(-z*z/2)*(8*z*z*z-12*z),
	  h4=n4*exp(-z*z/2)*(16*z*z*z*z - 48*z*z + 12),
	  h5=n5*exp(-z*z/2)*(32*z*z*z*z*z - 160*z*z*z + 120*z);
      
      double f0 = 0.25;
      
      Genfun::Variable x(0,2);
      Genfun::Variable t(1,2);
      
      PRectF & nr = rectHint();
      nr.setXmin(-10);
      nr.setXmax(10);
      nr.setYmin(-2);
      nr.setYmax(2);
      
      GENFUNCTION F=select(N,h0,h1,h2,h3,h4,h5);

      GENFUNCTION g= 
	type == REAL ?      F(x)*cos((1+N/2.0)*f0*t): 
	type == IMAGINARY ? F(x)*sin((1+N/2.0)*f0*t):
	F(x)*F(x);
      
      addFunction(g);
      
    }
    

    HOVisFunction(int N, int M, Type type):VisFunction(std::string("N=")
						       + toString(N) + std::string( " and M=") +toString(M) 
						       + std::string(
								     type == REAL      ?   std::string(" (real part)") : 
								     type == IMAGINARY ?   std::string(" (imaginary part)"): std::string(" (mag sq)")))
      {
	
	
	double 
	  n0=1/sqrt(sqrt(M_PI)              * 1),
	  n1=1/sqrt(sqrt(M_PI)*2            * 1*1),
	  n2=1/sqrt(sqrt(M_PI)*2*2          * 1*1*2),
	  n3=1/sqrt(sqrt(M_PI)*2*2*2        * 1*1*2*3),
	  n4=1/sqrt(sqrt(M_PI)*2*2*2*2      * 1*1*2*3*4),
	  n5=1/sqrt(sqrt(M_PI)*2*2*2*2*2    * 1*1*2*3*4*5);
	
	Variable z;
	Exp exp;
	
	GENFUNCTION
	  h0=n0*exp(-z*z/2),
	  h1=n1*exp(-z*z/2)*2*z,
	  h2=n2*exp(-z*z/2)*(4*z*z -2),
	  h3=n3*exp(-z*z/2)*(8*z*z*z-12*z),
	  h4=n4*exp(-z*z/2)*(16*z*z*z*z - 48*z*z + 12),
	  h5=n5*exp(-z*z/2)*(32*z*z*z*z*z - 160*z*z*z + 120*z);
	
	double f0 = 0.25;
	
	PRectF & nr = rectHint();
	nr.setXmin(-10);
	nr.setXmax(10);
	nr.setYmin(-2);
	nr.setYmax(2);
	
	
	Sin sin;
	Cos cos;
	
	Genfun::Variable x(0,2);
	Genfun::Variable t(1,2);

	GENFUNCTION F=select(N,h0,h1,h2,h3,h4,h5);
	GENFUNCTION G=select(M,h0,h1,h2,h3,h4,h5);
	
	GENFUNCTION R= 1/sqrt(2)*(F(x)*cos((1+N/2.0)*f0*t) +  G(x)*cos((1+M/2.0)*f0*t)); 
	GENFUNCTION I= 1/sqrt(2)*(F(x)*sin((1+N/2.0)*f0*t) +  G(x)*sin((1+M/2.0)*f0*t)); 
	
	
	if (type==REAL) {
	  addFunction(R);
	}
	else if (type==IMAGINARY) {
	  addFunction(I);
	}
	else {
	  addFunction(R*R+I*I);
	}
	
      }
};


#endif
