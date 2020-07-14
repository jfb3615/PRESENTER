#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Bessel.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
#include <sstream>
using namespace Genfun;

inline std::string toString(int i) {
  std::ostringstream o;
  o << i;
  return o.str();
}

class BesselFracVisFunction: public VisFunction {
 
public:
 
  BesselFracVisFunction():VisFunction("Bessel J of fractional order") {

    FractionalOrder::Bessel p(FractionalOrder::Bessel::FirstKind);
    const Cut<double> & cut = RealArg::Gt(0);

    Parameter *order      = new Parameter("Order", 1.0, 0.0, 10.0);
    p.n().connectFrom(order);
    

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(-1);
    nr.setYmax(1);

    addParameter(order);
    addFunction(p,&cut);

  }
};

extern "C" BesselFracVisFunction *create_BesselFracVisFunction()  {
  return new BesselFracVisFunction();
}
