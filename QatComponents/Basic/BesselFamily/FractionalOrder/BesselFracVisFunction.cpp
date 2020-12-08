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
    FractionalOrder::Bessel n(FractionalOrder::Bessel::FirstKind);
    const Cut<double> & cut = RealArg::Gt(0);

    Parameter *order      = new Parameter("Order", 1.0, 0.0, 10.0);
    AbsParameter *nOrder  = (-(*order)).clone(); 
    p.n().connectFrom(order);
    n.n().connectFrom(nOrder);
    

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(-1);
    nr.setYmax(1);

    addParameter(order);
    addFunction(p,&cut);
    addFunction(n,&cut);

  }
};

extern "C" BesselFracVisFunction *create_BesselFracVisFunction()  {
  return new BesselFracVisFunction();
}
