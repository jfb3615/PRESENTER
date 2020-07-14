#include "QatPlotting/VisFunction.h"
#include "QatPlotting/PRectF.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include <cmath>
using namespace Genfun;

class BoseEinsteinVisFunction: public VisFunction {
 
public:
 
  BoseEinsteinVisFunction():VisFunction("Bose Einstein Distribution") {

    Parameter       *T  = new Parameter("Tau", 0.1, 0.0, 1.0);

    Genfun::Exp exp;
    Genfun::Variable X;

    Genfun::GENFUNCTION fFD=1/(exp((X-1)/(*T))-1);


    addParameter(T);


    PRectF & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(0);
    nr.setYmax(2.0);

    addFunction(fFD);

  }
};


extern "C" BoseEinsteinVisFunction *create_BoseEinsteinVisFunction()  {
  return new BoseEinsteinVisFunction();
}
