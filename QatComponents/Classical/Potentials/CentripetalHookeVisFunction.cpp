#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class CentripetalHookeVisFunction: public VisFunction {
 
public:
 
  CentripetalHookeVisFunction():VisFunction("Centripetal + Hooke's Law") {


    Parameter *L = new Parameter("Angular Momentum", 2,     0,  10.0);
    Variable x;
    GENFUNCTION f = (*L)*(*L)/(x*x) +x*x;

    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(10);
    nr.setYmin(-100);
    nr.setYmax(100);

    const Cut<double> & cut=RealArg::Gt(0);

    addParameter(L);
    addFunction(f,&cut);

  }
};


extern "C" CentripetalHookeVisFunction *create_CentripetalHookeVisFunction()  {
  return new CentripetalHookeVisFunction();
}
