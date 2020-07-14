#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/VoigtDistribution.h"
#include <cmath>
using namespace Genfun;

class VoigtDistributionVisFunction: public VisFunction {
 
public:
 
  VoigtDistributionVisFunction():VisFunction("Voigt Distribution") {

    Parameter       *mean  = new Parameter("Mean", 0.0, -1.0, 1.0);
    Parameter       *sigma = new Parameter("Sigma", 0.5, 0.0, 1.0);
    Parameter       *delta  = new Parameter("Delta", 1.0, 0.0, 10.0);

    Genfun::VoigtDistribution voigt;
    Genfun::Variable X;

    voigt.sigma().connectFrom(sigma);
    voigt.delta().connectFrom(delta);

    addParameter(mean);
    addParameter(sigma);
    addParameter(delta);


    PRectF & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(0);
    nr.setYmax(2.0);

    addFunction(voigt(X-*mean));

  }
};


extern "C" VoigtDistributionVisFunction *create_VoigtDistributionVisFunction()  {
  return new VoigtDistributionVisFunction();
}
