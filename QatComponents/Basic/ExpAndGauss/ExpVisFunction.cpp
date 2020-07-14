#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class ExpVisFunction: public VisFunction {
 
public:
 
  ExpVisFunction():VisFunction("Exp") {

    Exp exp;
    
    PRectF & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(-1.5);
    nr.setYmax(1.5);

    addFunction(exp);

  }
};


extern "C" ExpVisFunction *create_ExpVisFunction()  {
  return new ExpVisFunction();
}
