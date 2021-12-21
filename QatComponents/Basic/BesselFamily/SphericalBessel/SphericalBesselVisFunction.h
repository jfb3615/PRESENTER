#ifndef _SphericalBesselVisFunction_h_
#define _SphericalBesselVisFunction_h_

#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Bessel.h"
#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
#include <sstream>
using namespace Genfun;

inline std::string toString(int i) {
  std::ostringstream o;
  o << i;
  return o.str();
}

class SphericalBesselVisFunction: public VisFunction {
 
public:
 
  SphericalBesselVisFunction(int l):VisFunction("j_"+toString(l)) {

    FractionalOrder::Bessel p(FractionalOrder::Bessel::FirstKind);
    p.n().setValue(l+0.5);
    Variable X;
    GENFUNCTION f = Sqrt()(M_PI/2/X)*p(X);
    const Cut<double> & cut = RealArg::Gt(0);
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(-1);
    nr.setYmax(1);

    addFunction(f,&cut);

  }
};
#endif

