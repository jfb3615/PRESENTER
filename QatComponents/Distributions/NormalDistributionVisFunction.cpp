#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/NormalDistribution.h"
#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace Genfun;

class NormalDistributionVisFunction: public VisFunction {
 
public:
 
  NormalDistributionVisFunction():VisFunction("NormalDistribution") {

    NormalDistribution gaussian;


    Parameter       *mean  = new Parameter("Mean", 0.0, -10.0, 10.0);
    Parameter       *sigma = new Parameter("Sigma", 0.5, 0.0, 5.0);

    gaussian.mean().connectFrom(mean);
    gaussian.sigma().connectFrom(sigma);

    PRectF & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(0);
    nr.setYmax(2.0);

    addParameter(mean);
    addParameter(sigma);
    addFunction(gaussian);

  }
};


extern "C" NormalDistributionVisFunction *create_NormalDistributionVisFunction()  {
  return new NormalDistributionVisFunction();
}
