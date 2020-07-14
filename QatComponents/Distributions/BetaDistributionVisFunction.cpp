#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/BetaDistribution.h"

using namespace std;

#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#          
#include <cmath>
using namespace Genfun;

class BetaDistributionVisFunction: public VisFunction {
 
public:
 
  BetaDistributionVisFunction():VisFunction("BetaDistribution ") {
    

    // THIS IS A TWO-PARAMETER FUNCTIO
    Parameter * a   = new Parameter("A", 1.0,  0.1,  10.0);
    Parameter * b  = new Parameter ("B", 1.0,  0.1,  10.0);
    addParameter(a);
    addParameter(b);


    Genfun::BetaDistribution f;

    f.alpha().connectFrom(a);
    f.beta().connectFrom(b);
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(1);
    nr.setYmin(0);
    nr.setYmax(4);


    addFunction(f);

  }
};


extern "C" BetaDistributionVisFunction *create_BetaDistributionVisFunction()  {
  return new BetaDistributionVisFunction();
}
