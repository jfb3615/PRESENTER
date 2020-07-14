#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Theta.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class ExponentialDistributionVisFunction: public VisFunction {
 
public:
 
  ExponentialDistributionVisFunction():VisFunction("Exponential Distribution") {

    Exp exp;
    Theta theta;

    Parameter *tau=new Parameter ("Tau", 1.0, 0.1, 10.0);
    Variable   X;
    

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(10);
    nr.setYmin(0);
    nr.setYmax(2.0);
    
    addParameter(tau);
    addFunction(theta(X)*exp(-X/(*tau))/(*tau));

  }
};


extern "C" ExponentialDistributionVisFunction *create_ExponentialDistributionVisFunction()  {
  return new ExponentialDistributionVisFunction();
}
