//---------------------------------------------------------------------------//
//                                                                           //
// --------------------------------------------------------------------------//

#ifndef _AssociatedLaguerreVisFunction_h_
#define _AssociatedLaguerreVisFunction_h_

#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/AssociatedLaguerrePolynomial.h"
#include "QatGenericFunctions/Variable.h"
#          
#include <cmath>
#include <sstream>
using namespace Genfun;

inline std::string toString(int i) {
  std::ostringstream o;
  o << i;
  return o.str();
}

class AssociatedLaguerreVisFunction: public VisFunction {
 
public:
 
  AssociatedLaguerreVisFunction(int l,int m):VisFunction("L_"+toString(l)+"^"+toString(m)) {

    AssociatedLaguerrePolynomial p(l,m);
    const Cut<double> & cut = RealArg::Gt(0);
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(10);
    nr.setYmin(-5);
    nr.setYmax(5);

    addFunction(p,&cut);

  }
};
#endif

