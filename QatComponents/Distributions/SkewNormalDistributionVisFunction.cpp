#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/NormalDistribution.h"
#include "QatGenericFunctions/FixedConstant.h"
#include "QatGenericFunctions/Erf.h"
#include <cmath>
using namespace Genfun;

class SkewNormalDistributionVisFunction: public VisFunction {
 
public:
 
  SkewNormalDistributionVisFunction():VisFunction("Skew Normal") {

    Parameter       *mean  = new Parameter("Mean", 0.0, -1.0, 1.0);
    Parameter       *sigma = new Parameter("Sigma", 0.5, 0.0, 1.0);
    Parameter       *lambda  = new Parameter("Lambda", 1.0, 0.0, 10.0);
    NormalDistribution        pdf;
    Erf                       erf;
    Variable        x;
    FixedConstant   I(1.0);
    GENFUNCTION     cdf=(1+erf)/2.0;
    GENFUNCTION     skewNormal = 2*pdf((x-*mean)/ (*sigma))*cdf(*lambda*(x-*mean)/(*sigma))/(*sigma);


    addParameter(mean);
    addParameter(sigma);
    addParameter(lambda);


    PRectF & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(0);
    nr.setYmax(2.0);

    addFunction(skewNormal);

  }
};


extern "C" SkewNormalDistributionVisFunction *create_SkewNormalDistributionVisFunction()  {
  return new SkewNormalDistributionVisFunction();
}
