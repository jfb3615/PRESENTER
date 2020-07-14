#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Power.h"
#include "QatGenericFunctions/ATan.h"
#include "QatGenericFunctions/Mod.h"
#include "QatGenericFunctions/Variable.h"   
#include <cmath>
using namespace Genfun;

class ForcedOscillatorPhaseVisFunction: public VisFunction {
 
public:
 
  ForcedOscillatorPhaseVisFunction():VisFunction("ForcedOscillatorPhase") {


    Parameter *q = new Parameter("Q", 10,  0,  100.0),  & Q=*q;
    Variable x;
    GENFUNCTION f = Mod(M_PI)(ATan()(x/(Q*(1+0.5/Q/Q - x*x)))+M_PI);
    
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(2);
    nr.setYmin(0);
    nr.setYmax(3);

    const Cut<double> & cut=RealArg::Gt(0);

    addParameter(q);
    addFunction(f,&cut);

  }
};


extern "C" ForcedOscillatorPhaseVisFunction *create_ForcedOscillatorPhaseVisFunction()  {
  return new ForcedOscillatorPhaseVisFunction();
}
