#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class CentripetalInverseRSquareVisFunction: public VisFunction {
 
public:
 
  CentripetalInverseRSquareVisFunction():VisFunction("Centripetal + Inv R Square") {


    Parameter *L = new Parameter("Angular Momentum", 0.5,     0,  1.0);
    Variable x;
    GENFUNCTION f = (*L)*(*L)/(x*x) - 1/x;

    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(4);
    nr.setYmin(-5);
    nr.setYmax(5);

    const Cut<double> & cut=RealArg::Gt(0);

    addParameter(L);
    addFunction(f,&cut);

  }
};


extern "C" CentripetalInverseRSquareVisFunction *create_CentripetalInverseRSquareVisFunction()  {
  return new CentripetalInverseRSquareVisFunction();
}
