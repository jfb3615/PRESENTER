#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/FixedConstant.h"
#include <cmath>
using namespace Genfun;

class H5VisFunction: public VisFunction {
 
public:
 
  H5VisFunction():VisFunction("H_5") {

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

    addFunction(h5);

  }
};


extern "C" H5VisFunction *create_H5VisFunction()  {
  return new H5VisFunction();
}
