#ifndef _NeumannVisFunction_h_
#define _NeumannVisFunction_h_

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

class NeumannVisFunction: public VisFunction {
  
public:
  
  NeumannVisFunction(int l):VisFunction("Y_"+toString(l)) {
    
    IntegralOrder::Bessel p(l,IntegralOrder::Bessel::SecondKind);
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

