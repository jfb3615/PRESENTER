#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Cos.h"
#include "QatGenericFunctions/ACos.h"
#include "QatGenericFunctions/Variable.h"

#include <cmath>
#include <complex>
using namespace Genfun;
#include "QatGenericFunctions/FixedConstant.h"
#include "QatGenericFunctions/AbsFunction.h"
typedef std::complex<double> Complex;
namespace Genfun {
class ComplexKFunction:public AbsFunction {

    FUNCTION_OBJECT_DEF(ComplexKFunction)

      public:
    
    // Constructor:
    ComplexKFunction();

    // Destructor:
    virtual ~ComplexKFunction();

    // Copy Constructor:
    ComplexKFunction (const ComplexKFunction & right);

    // Retrieve function value (here for function of one variable)
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}

    Parameter & sigma() {return _sigma;}
   
    // Derivative.  (here we provide)
    Derivative partial (unsigned int) const;

    // Does this function have an analytic derivative?  (here we say yes)
    virtual bool hasAnalyticDerivative() const {return true;}

  private:
    
    const ComplexKFunction & operator=(const ComplexKFunction &right)=delete;
    Parameter _sigma;
  };
}

namespace Genfun {

  FUNCTION_OBJECT_IMP(ComplexKFunction)

  // Constructor:
  ComplexKFunction::ComplexKFunction():AbsFunction(),
    _sigma("sigma", 1, 0, 10)
  {
  }
  
  // Destructor:
  ComplexKFunction::~ComplexKFunction (){
  }

  // Copy Constructor:
  ComplexKFunction::ComplexKFunction(const ComplexKFunction & right):AbsFunction(),
								     _sigma(right._sigma)
  {
  }
  
  // Function Call
  double ComplexKFunction::operator() ( double x) const {
    static const Complex  I(0.0,1.0);
    Complex epsilon(1,_sigma.getValue());
    Complex  nHat=sqrt(epsilon);
    return  exp(I*nHat*x).real();
  }

  
  // Partial Derivative
  Derivative ComplexKFunction::partial(unsigned int index) const {
    const AbsFunction & fPrime=FixedConstant(0);
    return Derivative(&fPrime);
  }
}

class SkinEffectVisFunction: public VisFunction {
 
public:
 
  SkinEffectVisFunction():VisFunction("SkinEffect") {


    Parameter *sigma = new Parameter("sigma", 0,     0,  10);
    ComplexKFunction fcn;
    fcn.sigma().connectFrom(sigma);


    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(-1.2);
    nr.setYmax(1.2);

    const Cut<double> & cut=RealArg::Gt(-0);

    addParameter(sigma);
    addFunction(fcn,&cut);

  }
};


extern "C" SkinEffectVisFunction *create_SkinEffectVisFunction()  {
  return new SkinEffectVisFunction();
}
