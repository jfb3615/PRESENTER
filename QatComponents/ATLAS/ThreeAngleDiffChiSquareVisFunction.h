#ifndef _ThreeAngleDiffChiSquareVisFunction_h_
#define _ThreeAngleDiffChiSquareVisFunction_h_
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

class ThreeAngleDiffChiSquareVisFunction: public VisFunction {
 
public:
 
  ThreeAngleDiffChiSquareVisFunction(int n):VisFunction("χ² 1 degree of freedom") {

    CumulativeChiSquare cc2(n);
    const Cut<double> & cut = RealArg::Gt(0.0);
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(-1);
    nr.setYmax(1);
    //
    // Below: this 0.01 is the bin width (of a particular histogram we look at)
    addFunction(cc2.prime(),&cut);
    //
  }
};
#endif

