//---------------------------------------------------------------------------//
//                                                                           //
// --------------------------------------------------------------------------//

#ifndef _HermiteVisFunction_h_
#define _HermiteVisFunction_h_

#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/HermitePolynomial.h"
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

class HermiteVisFunction: public VisFunction {
 
public:
 
  HermiteVisFunction(int l):VisFunction("L") {

    HermitePolynomial p(l);
    
    PRectF & nr = rectHint();
    nr.setXmin(-5);
    nr.setXmax(5);
    nr.setYmin(-20);
    nr.setYmax(20);

    addFunction(p);

  }
};
#endif

