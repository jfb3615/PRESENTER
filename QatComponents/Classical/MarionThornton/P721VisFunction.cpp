#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class P721VisFunction: public VisFunction {
 
public:
 
  P721VisFunction():VisFunction("Centripetal + Hooke's Law") {

    
    Parameter *g = new Parameter("Omega/Omega0", 2,     0,  4);
    Variable X;
    GENFUNCTION f = -(*g)*(*g)*Sin()(X*M_PI)*Sin()(X*M_PI)/2.0-Cos()(X*M_PI);

    
    PRectF & nr = rectHint();
    nr.setXmin(-1);
    nr.setXmax(1);
    nr.setYmin(-5);
    nr.setYmax(5);

    addParameter(g);
    addFunction(f);

  }
};


extern "C" P721VisFunction *create_P721VisFunction()  {
  return new P721VisFunction();
}
