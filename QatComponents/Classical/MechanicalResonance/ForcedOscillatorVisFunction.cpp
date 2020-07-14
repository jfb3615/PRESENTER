#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Power.h"
#include "QatGenericFunctions/Variable.h"   
#include <cmath>
using namespace Genfun;

class ForcedOscillatorVisFunction: public VisFunction {
 
public:
 
  ForcedOscillatorVisFunction():VisFunction("ForcedOscillator Only") {


    Parameter *q = new Parameter("Q", 10,  0,  100.0),  & Q=*q;
    Variable x;
    GENFUNCTION f = 1/Power(0.5)((x*x-1)*(x*x-1) + 1/(16.0*Q*Q*Q*Q) + x*x/(2.0*Q*Q));
    
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(2);
    nr.setYmin(0);
    nr.setYmax(10);

    const Cut<double> & cut=RealArg::Gt(0);

    addParameter(q);
    addFunction(f,&cut);

  }
};


extern "C" ForcedOscillatorVisFunction *create_ForcedOscillatorVisFunction()  {
  return new ForcedOscillatorVisFunction();
}
