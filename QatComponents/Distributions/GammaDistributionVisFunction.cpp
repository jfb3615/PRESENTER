#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Log.h"
#include "QatGenericFunctions/LGamma.h"
#include "QatGenericFunctions/Power.h"

using namespace std;

#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#          
#include <cmath>
using namespace Genfun;

class GammaDistributionVisFunction: public VisFunction {
 
public:
 
  GammaDistributionVisFunction():VisFunction("GammaDistribution ") {
    

    // THIS IS A TWO-PARAMETER FUNCTIO
    Parameter * a   = new Parameter("A", 2.5,  0.1,  10.0);
    Parameter * b  = new Parameter ("B", 0.4,  0.1,  10.0);
    addParameter(a);
    addParameter(b);


    GENFUNCTION f=Exp()(((*a)-1)*Log()(Variable())-Variable()/(*b)-LGamma()(*a)-(*a)*Log()(*b));
    
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(4);
    nr.setYmin(0);
    nr.setYmax(1);


    addFunction(f);

  }
};


extern "C" GammaDistributionVisFunction *create_GammaDistributionVisFunction()  {
  return new GammaDistributionVisFunction();
}
