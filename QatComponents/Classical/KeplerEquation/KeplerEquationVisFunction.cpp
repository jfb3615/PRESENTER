#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class KeplerEquationVisFunction: public VisFunction {
 
public:
 
  KeplerEquationVisFunction():VisFunction("Kepler equation") {


    Parameter *e = new Parameter("Eccentricity", 0.99,     0.0,  1.0);
    Variable x;
    GENFUNCTION f = x-(*e)*Sin()(x);

    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(4*M_PI);
    nr.setYmin(0);
    nr.setYmax(4*M_PI);

    const Cut<double> & cut=RealArg::Gt(0);

    addParameter(e);
    addFunction(f,&cut);

  }
};


extern "C" KeplerEquationVisFunction *create_KeplerEquationVisFunction()  {
  return new KeplerEquationVisFunction();
}
