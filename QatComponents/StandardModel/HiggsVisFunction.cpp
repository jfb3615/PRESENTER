#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class HiggsVisFunction: public VisFunction {
 
public:
 
  HiggsVisFunction():VisFunction("Higgs Potential") {

    Parameter *mu     = new Parameter("mu", 0.75, 0.0, 5.0);
    Parameter *lambda = new Parameter("lambda", 0.1, 0.0, 5.0);
    const Cut<double> & gt0 = RealArg::Gt(0);

    Variable X;
    GENFUNCTION higgsPotential = -(*mu)*(*mu)*X*X + (*lambda)*X*X*X*X;
    

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(5);
    nr.setYmin(-1);
    nr.setYmax(4);

    addParameter(mu);
    addParameter(lambda);

    addFunction(higgsPotential, &gt0);

  }
};

extern "C" HiggsVisFunction *create_HiggsVisFunction()  {
  return new HiggsVisFunction();
}
