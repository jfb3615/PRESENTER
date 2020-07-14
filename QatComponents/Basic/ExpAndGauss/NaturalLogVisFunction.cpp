#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Log.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class NaturalLogVisFunction: public VisFunction {
 
public:
 
  NaturalLogVisFunction():VisFunction("Natural Log") {

    Log ln;
    const Cut<double> & gt0 = RealArg::Gt(1.0E-10);

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(100);
    nr.setYmin(-10);
    nr.setYmax(10);

    addFunction(ln,&gt0);

  }
};


extern "C" NaturalLogVisFunction *create_NaturalLogVisFunction()  {
  return new NaturalLogVisFunction();
}
