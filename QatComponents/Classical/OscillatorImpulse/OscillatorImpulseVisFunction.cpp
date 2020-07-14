#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Power.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/FixedConstant.h"
#include "QatGenericFunctions/Theta.h"
#include <cmath>
#include <sstream>
#include <string>
using namespace Genfun;

class OscillatorImpulseVisFunction: public VisFunction {
 
public:
 
  OscillatorImpulseVisFunction():VisFunction("Pulsed Oscillator") {

    Variable X;

    FixedConstant            I(1.0);
    Parameter *beta       = new Parameter ("Beta", 1.0, 0.0, 5.0);
    Parameter *gamma      = new Parameter ("Gamma", 1.0, 0.0, 5.0);

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax( 20);
    nr.setYmin(-1);
    nr.setYmax(1);

    addParameter(beta);
    addParameter(gamma);

    


    GENFUNCTION Impulse     = Theta()(X)*Exp()(-*gamma*X);
    GENFUNCTION Response    = Theta()(X)*Exp()(-*beta*X)*Sin()(X);
    GENFUNCTION Convolution = 1.0/(Power(2.0)(*gamma-*beta)+1.0)*(
								  Exp()(-*gamma*X)-
								  Exp()(-*beta*X) *
								  (Cos()(X)-(*gamma-*beta)*Sin()(X)));
						  
    const Cut<double> & cut = RealArg::Gt(0.0);

    addFunction(Convolution,&cut);
    addFunction(Impulse, &cut);
    addFunction(Response,&cut);

    
  }
};

extern "C" OscillatorImpulseVisFunction *create_OscillatorImpulseVisFunction()  {
  return new OscillatorImpulseVisFunction();
}
