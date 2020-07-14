#ifndef _HJVISFUNCTION_H_
#define _HJVISFUNCTION_H_

#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/ASin.h"
#include "QatGenericFunctions/ATan.h"
#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include <sstream>
#include <cmath>
#include <string>
#include <stdexcept>
using namespace Genfun;



class HJVisFunction: public VisFunction {
 
public:
 
  enum Type {REAL,IMAGINARY, MAGSQ};
  
 HJVisFunction(Type type):VisFunction(type == REAL      ?   std::string(" (real part)") : type==IMAGINARY ? std::string(" (imaginary part)") : std::string("mag squared"))
    {
      using namespace Genfun;
      Genfun::Variable x(0,2);
      Genfun::Variable t(1,2);
      Sqrt     sqrt;
      ATan     atan;
      ASin     asin;
      Exp      exp;
      Sin sin;
      Cos cos;
      
      //GENFUNCTION W=0.5*(x*sqrt(1-x*x)+ atan(x/sqrt(1-x*x)));
      GENFUNCTION W=0.5*(x*sqrt(1-x*x)+ asin(x));
      GENFUNCTION S=W-0.5*t;
  
      PRectF & nr = rectHint();
      nr.setXmin(-1);
      nr.setXmax(1);
      nr.setYmin(-2);
      nr.setYmax(2);

      GENFUNCTION R=cos(S);
      GENFUNCTION I=sin(S);
      GENFUNCTION M2=S;
     
      GENFUNCTION g= type == REAL      ? R: type==IMAGINARY ? I : M2; 
      addFunction(g);
    }
    

};


#endif
