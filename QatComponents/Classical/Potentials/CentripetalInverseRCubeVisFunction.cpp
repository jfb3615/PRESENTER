#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class CentripetalInverseRCubeVisFunction: public VisFunction {
 
public:
 
  CentripetalInverseRCubeVisFunction():VisFunction("Centripetal + Inv R Cube") {


    Parameter *L = new Parameter("Angular Momentum", 2,     0,  10.0);
    Variable x;
    GENFUNCTION f = (*L)*(*L)/(x*x) - 1/x/x;

    
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


extern "C" CentripetalInverseRCubeVisFunction *create_CentripetalInverseRCubeVisFunction()  {
  return new CentripetalInverseRCubeVisFunction();
}
