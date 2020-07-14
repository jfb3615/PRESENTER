#ifndef _ModifiedBesselVisFunction_h_
#define _ModifiedBesselVisFunction_h_

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

class ModifiedBesselVisFunction: public VisFunction {
 
public:
 
  ModifiedBesselVisFunction(int l):VisFunction("K_"+toString(l)) {

    IntegralOrder::Bessel p(l,IntegralOrder::Bessel::ModifiedSecondKind);
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

