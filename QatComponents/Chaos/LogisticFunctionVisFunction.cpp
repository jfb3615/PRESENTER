#include "QatGenericFunctions/Variable.h"
#include <cmath>
using namespace std;
#include "QatPlotting/VisFunction.h"
#include "QatPlotting/PRectF.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"

#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include <vector>
namespace Genfun {

  class LogisticFunction : public AbsFunction  {

    FUNCTION_OBJECT_DEF(LogisticFunction)

      public:

    // Constructor
    LogisticFunction();

    // Copy constructor
    LogisticFunction(const LogisticFunction &right);
  
    // Destructor
    virtual ~LogisticFunction();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & arg) const {return operator() (arg[0]);}
  
    // Get the starting value of the LogisticFunction
    Parameter & x0(); 
    const Parameter & x0() const; 

    // Get the control parameter of the LogisticFunction
    Parameter & a();
    const Parameter & a() const;

  private:

    // It is illegal to assign an adjustable constant
    const LogisticFunction & operator=(const LogisticFunction &right);

    // Here is the decay constant
    Parameter _x0;

    // Here is the sigma
    Parameter _a;

    // A vector of values.
    mutable std::vector<double> fx;
    // Some cache:
    mutable double __a, __x0;

  };
} // namespace Genfun


#define MAXRANGE 1000

namespace Genfun {
FUNCTION_OBJECT_IMP(LogisticFunction)

LogisticFunction::LogisticFunction():
  _x0("X0", 0.0,0.0,1.0),
  _a("MU",0.5,0.25, 4.0)
{}

LogisticFunction::~LogisticFunction() {
}

LogisticFunction::LogisticFunction(const LogisticFunction & right):
AbsFunction(right),
_x0(right._x0),
_a(right._a)
{
}

double LogisticFunction::operator() (double x) const {
  int i  = (int) (x+0.5), &back = i, end=back+1;

  if (i<0 || i>MAXRANGE) {
    return 0;
  }
  else {
    // Is the vector of values stale?
    if (__a!=_a.getValue() || __x0!=_x0.getValue()) {

      // Empty the vector
      fx.erase(fx.begin(),fx.end());

      // And update the cache.
      __a  = _a.getValue(); 
      __x0 = _x0.getValue();
      
    }
    

    if (fx.empty()) fx.push_back(__x0);

    while (fx.size()<size_t(end)) {
      double v = fx.back();
      fx.push_back(__a*v*(1.0-v));
    }
    
    return fx[i];
  }

  
}

Parameter & LogisticFunction::x0() {
  return _x0;
}

Parameter & LogisticFunction::a() {
  return _a;
}

const Parameter & LogisticFunction::x0() const {
  return _x0;
}

const Parameter & LogisticFunction::a() const {
  return _a;
}


} // namespace Genfun

          
using namespace Genfun;
class LogisticFunctionVisFunction: public VisFunction {
 
public:
 
  LogisticFunctionVisFunction():VisFunction("LogisticFunction") {
    

    Parameter *A = new Parameter("MU", 0.5, 0.25, 4);
    Parameter *X0 = new Parameter("X0", 0, -1.0, 1.0);
    addParameter(A);
    addParameter(X0);

    Genfun::LogisticFunction f;
    f.x0().connectFrom(X0);
    f.a().connectFrom(A);

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(-2);
    nr.setYmax(2);
    
    
    addFunction(f);

  }
};


extern "C" LogisticFunctionVisFunction *create_LogisticFunctionVisFunction()  {
  return new LogisticFunctionVisFunction();
}
