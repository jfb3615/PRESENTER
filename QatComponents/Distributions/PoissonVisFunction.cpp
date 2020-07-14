#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Log.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Mod.h"
#include "QatGenericFunctions/LGamma.h"
#include <cmath>
using namespace Genfun;

class PoissonVisFunction: public VisFunction {
 
public:
 
  PoissonVisFunction():VisFunction("Poisson") {

 
    Parameter       *mean  = new Parameter("Mean", 5.0, 0.001, 50);
    GENPARAMETER     lm=*mean;
    Variable        X;
    GENFUNCTION     XM=X-Mod(1.0)(X);
    GENFUNCTION f=Exp()(-lm - LGamma()(XM+1)+XM*Log()(lm));

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(50);
    nr.setYmin(0);
    nr.setYmax(2.0);

    addParameter(mean);
    addFunction(f);

  }
};


extern "C" PoissonVisFunction *create_PoissonVisFunction()  {
  return new PoissonVisFunction();
}
