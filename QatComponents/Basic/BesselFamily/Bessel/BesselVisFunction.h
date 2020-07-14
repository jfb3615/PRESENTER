#ifndef _BesselVisFunction_h_
#define _BesselVisFunction_h_

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

class BesselVisFunction: public VisFunction {
 
public:
 
  BesselVisFunction(int l):VisFunction("J_"+toString(l)) {

    IntegralOrder::Bessel p(l,IntegralOrder::Bessel::FirstKind);
    const Cut<double> & cut = RealArg::Gt(0);
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(-1);
    nr.setYmax(1);

    addFunction(p,&cut);

  }
};
#endif

