#ifndef _AssociatedLegendreVisFunction_h_
#define _AssociatedLegendreVisFunction_h_

#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/AssociatedLegendre.h"
#include "QatGenericFunctions/Variable.h"
          
#include <cmath>
#include <sstream>
using namespace Genfun;

inline std::string toString(int i) {
  std::ostringstream o;
  o << i;
  return o.str();
}

class AssociatedLegendreVisFunction: public VisFunction {
 
public:
 
  AssociatedLegendreVisFunction(int l,int m):VisFunction("P_"+toString(l)+"^"+toString(m)) {

    AssociatedLegendre p(l,m,TWIDDLE);
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

