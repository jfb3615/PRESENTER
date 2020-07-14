#include "QatGenericFunctions/Variable.h"
#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include <vector>
#include <cmath>
using namespace std;
namespace Genfun {

  class SitePerc : public AbsFunction  {

    FUNCTION_OBJECT_DEF(SitePerc)

      public:

    // Constructor
    SitePerc();

    // Copy constructor
    SitePerc(const SitePerc &right);
  
    // Destructor
    virtual ~SitePerc();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & arg) const {return operator() (arg[0]);}
  
    // Get the starting value of the SitePerc
    Parameter & x0(); 
    const Parameter & x0() const; 

  private:

    // It is illegal to assign an adjustable constant
    const SitePerc & operator=(const SitePerc &right);

    // Here is the decay constant
    Parameter _x0;

    // A vector of values.
    mutable std::vector<double> fx;
    // Some cache:
    mutable double __x0;

  };
} // namespace Genfun


#define MAXRANGE 1000

namespace Genfun {
FUNCTION_OBJECT_IMP(SitePerc)

SitePerc::SitePerc():
  _x0("X0", 0.6175,0.6150,0.6200)
{}

SitePerc::~SitePerc() {
}

SitePerc::SitePerc(const SitePerc & right):
AbsFunction(right),
_x0(right._x0)
{
}

double SitePerc::operator() (double x) const {
  int i  = (int) (x+0.5), &back = i, end=back+1;

  if (i<0 || i>MAXRANGE) {
    return 0;
  }
  else {
    // Is the vector of values stale?
    if (__x0!=_x0.getValue()) {

      // Empty the vector
      fx.erase(fx.begin(),fx.end());

      // And update the cache.
      __x0 = _x0.getValue();
      
    }
    

    if (fx.empty()) fx.push_back(__x0);

    while (fx.size()<size_t(end)) {
      double v = fx.back();
      fx.push_back(v*v*(2.0-v*v));
    }
    
    return fx[i];
  }

  
}

Parameter & SitePerc::x0() {
  return _x0;
}


const Parameter & SitePerc::x0() const {
  return _x0;
}



} // namespace Genfun

          
using namespace Genfun;
class SitePercVisFunction: public VisFunction {
 
public:
 
  SitePercVisFunction():VisFunction("SitePerc") {
    

    Parameter *X0 = new Parameter("X0", 0.61, 0.60, 0.62);
    addParameter(X0);

    Genfun::SitePerc f;
    f.x0().connectFrom(X0);

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(0);
    nr.setYmax(1);
    
    
    addFunction(f);

  }
};


extern "C" SitePercVisFunction *create_SitePercVisFunction()  {
  return new SitePercVisFunction();
}
