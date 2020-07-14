#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/ACos.h"
#include "QatGenericFunctions/Variable.h"
#          
#include <cmath>
using namespace Genfun;

class HeadlightVisFunction: public VisFunction {
 
public:
 
  HeadlightVisFunction():VisFunction("Headlight Only") {


    Parameter *g = new Parameter("E/m", 1,     1,  100.0);
    GENPARAMETER gamma2=(*g)*(*g);
    GENPARAMETER beta2 = 1.0-1.0/gamma2;
    Variable x;
    Sin sin;
    Cos cos;
    ACos acos;
    GENFUNCTION f = ((x*x-1)+gamma2*(beta2-x*x))/(gamma2*(1-beta2*x*x)); 
    
    
    PRectF & nr = rectHint();
    nr.setXmin(-1);
    nr.setXmax(1);
    nr.setYmin(-1.2);
    nr.setYmax(1.2);

    const Cut<double> & cut=RealArg::Gt(-1) && RealArg::Lt(1);

    addParameter(g);
    addFunction(f,&cut);

  }
};


extern "C" HeadlightVisFunction *create_HeadlightVisFunction()  {
  return new HeadlightVisFunction();
}
