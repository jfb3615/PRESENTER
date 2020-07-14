#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/LGamma.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class LGammaVisFunction: public VisFunction {
 
public:
 
  LGammaVisFunction():VisFunction("Log Gamma") {

    LGamma logGamma;
    const Cut<double> & gt0 = RealArg::Gt(1E-10);

    PRectF & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(-100);
    nr.setYmax(100);

    addFunction(logGamma,&gt0);

  }
};


extern "C" LGammaVisFunction *create_LGammaVisFunction()  {
  return new LGammaVisFunction();
}
