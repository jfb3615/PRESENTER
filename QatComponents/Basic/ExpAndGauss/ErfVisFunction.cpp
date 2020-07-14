#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Erf.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class ErfVisFunction: public VisFunction {
 
public:
 
  ErfVisFunction():VisFunction("Erf") {

    Erf erf;
    Variable x;

    Parameter *sigma = new Parameter ("Sigma", 1.0, 0.25, 10.0);
    
    PRectF & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(-1.5);
    nr.setYmax(1.5);

    addParameter(sigma);
    addFunction(erf(x/(*sigma)));

  }
};


extern "C" ErfVisFunction *create_ErfVisFunction()  {
  return new ErfVisFunction();
}
