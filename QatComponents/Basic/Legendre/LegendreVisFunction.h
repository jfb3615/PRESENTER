#ifndef _LegendreVisFunction_h_
#define _LegendreVisFunction_h_

#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/LegendrePolynomial.h"          
#include <cmath>
#include <sstream>
using namespace Genfun;

inline std::string toString(int i) {
  std::ostringstream o;
  o << i;
  return o.str();
}

class LegendreVisFunction: public VisFunction {
 
public:
 
  LegendreVisFunction(int l):VisFunction("P_"+toString(l)) {

    LegendrePolynomial p(l);
    const Cut<double> & cut = RealArg::Gt(-1) && RealArg::Lt(1);
    
    PRectF & nr = rectHint();
    nr.setXmin(-1);
    nr.setXmax(1);
    nr.setYmin(-5);
    nr.setYmax(5);

    addFunction(p,&cut);

  }
};
#endif

