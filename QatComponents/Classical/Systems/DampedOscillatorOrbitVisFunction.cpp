#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Bessel.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Power.h"
#include "QatGenericFunctions/RKIntegrator.h"
#include <cmath>
#include <sstream>
#include <string>
using namespace Genfun;

class DampedOscillatorOrbitVisFunction: public VisFunction {
 
public:
 
  DampedOscillatorOrbitVisFunction():VisFunction("Damped Oscillator Orbit") {

    Variable U(1,2),X(0,2);

    FixedConstant            I(1.0);
    Parameter *zeta      = integrator.createControlParameter ("Zeta", 0.0 , 0.0, 5.0);

    GENFUNCTION  dXdt=U;
    GENFUNCTION  dUdt=-(2**zeta*U+X);
    
    Parameter *x0=integrator.addDiffEquation (&dXdt, "X0", 1.0 , -4.0, 4.0);   
    Parameter *u0=integrator.addDiffEquation (&dUdt, "U0", 0.0 , -4.0, 4.0);   
    
    PRectF & nr = rectHint();
    nr.setXmin(-1.5);
    nr.setXmax(1.5);
    nr.setYmin(-1.5);
    nr.setYmax(+1.5);

    Parameter *indX0=new Parameter(x0->getName(),x0->getValue(), x0->getLowerLimit(), x0->getUpperLimit());
    Parameter *indU0=new Parameter(u0->getName(),u0->getValue(), u0->getLowerLimit(), u0->getUpperLimit());
    Parameter *indZeta=new Parameter(zeta->getName(),zeta->getValue(), zeta->getLowerLimit(), zeta->getUpperLimit());
    x0->connectFrom(indX0);
    u0->connectFrom(indU0);
    zeta->connectFrom(indZeta);
   
    addParameter(indX0);
    addParameter(indU0);
    addParameter(indZeta);

    Variable T;
    addFunction((*integrator.getFunction(X.index()))(100*T));
    addFunction((*integrator.getFunction(U.index()))(100*T));

    setMode(VisFunction::ORBIT);
  }
  private:
      Genfun::RKIntegrator integrator;
    
};

extern "C" DampedOscillatorOrbitVisFunction *create_DampedOscillatorOrbitVisFunction()  {
  return new DampedOscillatorOrbitVisFunction();
}
