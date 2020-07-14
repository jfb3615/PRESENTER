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

  class SitePerc3x3 : public AbsFunction  {

    FUNCTION_OBJECT_DEF(SitePerc3x3)

      public:

    // Constructor
    SitePerc3x3();

    // Copy constructor
    SitePerc3x3(const SitePerc3x3 &right);
  
    // Destructor
    virtual ~SitePerc3x3();
  
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & arg) const {return operator() (arg[0]);}
  
    // Get the starting value of the SitePerc3x3
    Parameter & x0(); 
    const Parameter & x0() const; 

  private:

    // It is illegal to assign an adjustable constant
    const SitePerc3x3 & operator=(const SitePerc3x3 &right);

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
FUNCTION_OBJECT_IMP(SitePerc3x3)

SitePerc3x3::SitePerc3x3():
  _x0("X0", 0.6175,0.6150,0.6200)
{}

SitePerc3x3::~SitePerc3x3() {
}

SitePerc3x3::SitePerc3x3(const SitePerc3x3 & right):
AbsFunction(right),
_x0(right._x0)
{
}

double SitePerc3x3::operator() (double x) const {
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
      double w = 1-v;
      fx.push_back(
		   pow(v,9)+
		   9*pow(v,8)*pow(w,1)+
		   36*pow(v,7)*pow(w,2)+
		   67*pow(v,6)*pow(w,3)+
		   59*pow(v,5)*pow(w,4)+
		   22*pow(v,4)*pow(w,5)+
		   3*pow(v,3)*pow(w,6)
);
    }
    
    return fx[i];
  }

  
}

Parameter & SitePerc3x3::x0() {
  return _x0;
}


const Parameter & SitePerc3x3::x0() const {
  return _x0;
}



} // namespace Genfun

          
using namespace Genfun;
class SitePerc3x3VisFunction: public VisFunction {
 
public:
 
  SitePerc3x3VisFunction():VisFunction("SitePerc3x3") {
    

    Parameter *X0 = new Parameter("X0", 0.61, 0.60, 0.62);
    addParameter(X0);

    Genfun::SitePerc3x3 f;
    f.x0().connectFrom(X0);

    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(20);
    nr.setYmin(0);
    nr.setYmax(1);
    
    
    addFunction(f);

  }
};


extern "C" SitePerc3x3VisFunction *create_SitePerc3x3VisFunction()  {
  return new SitePerc3x3VisFunction();
}
