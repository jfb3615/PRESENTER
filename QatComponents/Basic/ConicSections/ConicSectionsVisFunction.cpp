#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include <cmath>
using namespace Genfun;

class ConicSectionsVisFunction: public VisFunction {
 
public:
 
  ConicSectionsVisFunction():VisFunction("Conic Sections") {

    Parameter *e = new Parameter("e", 0, 0, 4.0);

    Variable THETA;
    GENFUNCTION XTHETA = 2*M_PI*THETA;
    GENFUNCTION R=1/(1+(*e)*Cos()(XTHETA));
    GENFUNCTION X=R*Cos()(XTHETA);
    GENFUNCTION Y=R*Sin()(XTHETA);
    
    
    PRectF & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(-10);
    nr.setYmax(10);

    addParameter(e);

    
    addFunction(X);
    addFunction(Y);
    setMode(VisFunction::ORBIT);
  }
};

extern "C" ConicSectionsVisFunction *create_ConicSectionsVisFunction()  {
  return new ConicSectionsVisFunction();
}
