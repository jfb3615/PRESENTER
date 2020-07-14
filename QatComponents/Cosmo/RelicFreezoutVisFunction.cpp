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

class RelicFreezoutVisFunction: public VisFunction {
 
public:
 
  RelicFreezoutVisFunction():VisFunction("Cosmic relic Freezout function") {

    Variable Z(1,2),X(0,2);

    FixedConstant            I(1.0);
    Exp                      exp;
    Parameter *lKappa      = integrator.createControlParameter ("Log 10 Kappa", 0.0 , -6, 6.0);

    GENFUNCTION  dXdx=   I%I;
    Parameter *x0=integrator.addDiffEquation (&dXdx, "X", 1.0 , 0.5, 1.5);   
    
    AbsParameter *kappa     = exp(log(10.0)* (*lKappa)).clone();
    AbsParameter *_z0       = ((*kappa)*Power(2)((*x0)*(*x0)*IntegralOrder::Bessel(2,IntegralOrder::Bessel::ModifiedSecondKind)(*x0))).clone();
    GENFUNCTION  dZdx=  -1/X/X*(Z*Z-(*kappa)*Power(2)(X*X*IntegralOrder::Bessel(2,IntegralOrder::Bessel::ModifiedSecondKind)(X)));
    
    
    Parameter *z0=integrator.addDiffEquation (&dZdx, "Z", 0.0 , -100, 100);

    z0->connectFrom(_z0); // Z0 is not independent.  It depends on kappa and x0.
    

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(0);
    nr.setYmax(1000);

    const Cut<double> & cut = RealArg::Gt(0.5);
    Parameter *indX0=new Parameter(x0->getName(),x0->getValue(), x0->getLowerLimit(), x0->getUpperLimit());
    Parameter *indKP=new Parameter(lKappa->getName(),lKappa->getValue(), lKappa->getLowerLimit(), lKappa->getUpperLimit());
    x0->connectFrom(indX0);
    lKappa->connectFrom(indKP);
    addParameter(indKP);
    addParameter(indX0);
    addFunction(*integrator.getFunction(Z.index()),&cut);

    
  }
  private:
      Genfun::RKIntegrator integrator;
    
};

extern "C" RelicFreezoutVisFunction *create_RelicFreezoutVisFunction()  {
  return new RelicFreezoutVisFunction();
}
