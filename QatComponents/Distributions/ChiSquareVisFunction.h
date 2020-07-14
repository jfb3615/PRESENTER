#ifndef _ChiSquareVisFunction_h_
#define _ChiSquareVisFunction_h_
#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/CumulativeChiSquare.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
#include <sstream>
using namespace Genfun;

inline std::string toString(int i) {
  std::ostringstream o;
  o << i;
  return o.str();
}

class ChiSquareVisFunction: public VisFunction {
 
public:
 
  ChiSquareVisFunction(int n):VisFunction("Prob chisquare, "+toString(n)+" degrees of freedom") {

    CumulativeChiSquare cc2(n);
    const Cut<double> & cut = RealArg::Gt(0.0);
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(-1);
    nr.setYmax(1);

    addFunction(cc2,&cut);

  }
};
#endif

