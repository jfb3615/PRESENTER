#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/Variable.h"

#include <cmath>
#include <complex>
using namespace Genfun;
#include "QatGenericFunctions/FixedConstant.h"
#include "QatGenericFunctions/AbsFunction.h"
typedef std::complex<double> Complex;

class DampedOscVisFunction: public VisFunction {
 
public:
 
  DampedOscVisFunction():VisFunction("DampedOsc") {


    Parameter *beta = new Parameter    ("beta", 0,     0,1);
    Parameter &b=*beta;
    GENPARAMETER  omega1=Genfun::Sqrt()(1-(*beta)*(*beta));
    Variable    T;
    GENFUNCTION X=Exp()(-b*T)*Cos()(omega1*T);

    GENFUNCTION E=0.5*(X*X+X.prime()*X.prime());
    GENFUNCTION EPrime=E.prime();
    
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(-1.2);
    nr.setYmax(1.2);

    const Cut<double> & cut=RealArg::Gt(-0);

    addParameter(beta);
    addFunction(E,&cut);
    addFunction(EPrime,&cut);

  }
};


extern "C" DampedOscVisFunction *create_DampedOscVisFunction()  {
  return new DampedOscVisFunction();
}
