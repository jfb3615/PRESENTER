#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/IncompleteGamma.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class IncompleteGammaVisFunction: public VisFunction {
 
public:
 
  IncompleteGammaVisFunction():VisFunction("Incomplete Gamma") {

    Parameter *a = new Parameter("a", 1.0, 0.5, 5.0);
    IncompleteGamma incompleteGamma;
    const Cut<double> & gt0 = RealArg::Gt(0);

    incompleteGamma.a().connectFrom(a);

    PRectF & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(-1.5);
    nr.setYmax(1.5);

    addParameter(a);

    
    addFunction(incompleteGamma, &gt0);

  }
};

extern "C" IncompleteGammaVisFunction *create_IncompleteGammaVisFunction()  {
  return new IncompleteGammaVisFunction();
}
