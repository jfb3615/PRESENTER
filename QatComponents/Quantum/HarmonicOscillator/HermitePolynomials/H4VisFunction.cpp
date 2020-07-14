#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/FixedConstant.h"
#include <cmath>
using namespace Genfun;

class H4VisFunction: public VisFunction {
 
public:
 
  H4VisFunction():VisFunction("H_4") {

    Variable x;
    FixedConstant I(1.0);
    GENFUNCTION 
      h0 = I,
      h1 = 2*x,
      h2 = 4*x*x -2,
      h3 = 8*x*x*x-12*x,
      h4 = 16*x*x*x*x - 48*x*x + 12,
      h5 = 32*x*x*x*x*x - 160*x*x*x + 120*x;

    PRectF & nr = rectHint();
    nr.setXmin(-2.5);
    nr.setXmax(2.5);
    nr.setYmin(-200);
    nr.setYmax(200);

    addFunction(h4);

  }
};


extern "C" H4VisFunction *create_H4VisFunction()  {
  return new H4VisFunction();
}
