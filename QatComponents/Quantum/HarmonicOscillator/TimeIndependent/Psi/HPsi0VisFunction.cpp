#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/FixedConstant.h"
#include <cmath>
using namespace Genfun;

class HPsi0VisFunction: public VisFunction {
 
public:
 
  HPsi0VisFunction():VisFunction("Psi_0") {

    double 
      n0=1/sqrt(sqrt(M_PI)              * 1),
      n1=1/sqrt(sqrt(M_PI)*2            * 1*1),
      n2=1/sqrt(sqrt(M_PI)*2*2          * 1*1*2),
      n3=1/sqrt(sqrt(M_PI)*2*2*2        * 1*1*2*3),
      n4=1/sqrt(sqrt(M_PI)*2*2*2*2      * 1*1*2*3*4),
      n5=1/sqrt(sqrt(M_PI)*2*2*2*2*2    * 1*1*2*3*4*5);
    
    Variable x;
    FixedConstant I(1.0);
    Exp exp;

    GENFUNCTION 
      h0 = n0*exp(-x*x/2)*I,
      h1 = n1*exp(-x*x/2)*2*x,
      h2 = n2*exp(-x*x/2)*(4*x*x -2),
      h3 = n3*exp(-x*x/2)*(8*x*x*x-12*x),
      h4 = n4*exp(-x*x/2)*(16*x*x*x*x - 48*x*x + 12),
      h5 = n5*exp(-x*x/2)*(32*x*x*x*x*x - 160*x*x*x + 120*x);

    PRectF  & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(-1);
    nr.setYmax(1);
    

    addFunction(h0);

  }
};


extern "C" HPsi0VisFunction *create_HPsi0VisFunction()  {
  return new HPsi0VisFunction();
}
