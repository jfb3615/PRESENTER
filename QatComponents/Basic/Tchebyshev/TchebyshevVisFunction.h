//---------------------------------------------------------------------------//
//                                                                           //
// --------------------------------------------------------------------------//

#ifndef _TchebyshevVisFunction_h_
#define _TchebyshevVisFunction_h_

#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/TchebyshevPolynomial.h"
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

class TchebyshevVisFunction: public VisFunction {
 
public:
 
  TchebyshevVisFunction(int l):VisFunction("L") {

    TchebyshevPolynomial p(l);
    
    PRectF & nr = rectHint();
    nr.setXmin(-1);
    nr.setXmax(1);
    nr.setYmin(-2);
    nr.setYmax(2);

    const Cut<double>  &cut=RealArg::Gt(-1.0) && RealArg::Lt(1.0);

    addFunction(p,&cut);

  }
};
#endif

