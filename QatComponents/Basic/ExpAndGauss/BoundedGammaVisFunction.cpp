#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/IncompleteGamma.h"
#include <cmath>
using namespace std;

namespace Genfun {

  /**
   * @author
   * @ingroup genfun
   */
  class BoundedGammaDistribution : public AbsFunction  {
    
    FUNCTION_OBJECT_DEF(BoundedGammaDistribution)
      
      public:
    
    // Constructor
    BoundedGammaDistribution();
    
    // Copy constructor
    BoundedGammaDistribution(const BoundedGammaDistribution &right);
    
    // Destructor
    virtual ~BoundedGammaDistribution();
    
    // Retreive function value
    virtual double operator ()(double argument) const;
    virtual double operator ()(const Argument & a) const {return operator() (a[0]);}
    
    // Get the paramter peak
    Parameter & peak(); 

    // Get the parameter width
    Parameter & width(); 

    // Get the limit parameter.
    Parameter & limit(); 
    
  private:
    
    // It is illegal to assign an adjustable constant
    const BoundedGammaDistribution & operator=(const BoundedGammaDistribution &right);
    
    // Here are the two parameters alpha and beta:

    Parameter       _peak;
    Parameter       _width;
    Parameter       _limit;

    mutable IncompleteGamma _IGamma;

  };

  FUNCTION_OBJECT_IMP(BoundedGammaDistribution)


    BoundedGammaDistribution::BoundedGammaDistribution():
      _peak("peak",    1.0, 1.0, 100),
      _width ("width", 0.0, 0,   100),
      _limit("limit",0.0, 10.0, 1.0E30)
  {}
  
  BoundedGammaDistribution::BoundedGammaDistribution(const BoundedGammaDistribution & right):
    AbsFunction(),
    _peak(right._peak),
    _width (right._width),
    _limit(right._limit),
    _IGamma(right._IGamma)
  {
  }
  
  BoundedGammaDistribution::~BoundedGammaDistribution() {
  }
  
  double BoundedGammaDistribution::operator() (double x) const {
    
    double R = _width.getValue()*_width.getValue()/_peak.getValue()/_peak.getValue();
    double alpha = (1 + sqrt(1+4*R))/2.0/R;
    double beta  = _peak.getValue()/alpha;
     _IGamma.a().setValue(alpha+1);
    double retVal= pow(x,alpha)*
      exp(-x/beta)/pow(beta,(alpha+1))/
      _IGamma(_limit.getValue()/beta)/exp(lgamma(alpha+1));
    if (!finite(retVal)) retVal=0.0;
    return retVal;
  }
  
  Parameter & BoundedGammaDistribution::peak() {
    return _peak;
  }
  
  
  Parameter & BoundedGammaDistribution::width() {
    return _width;
  }
  
  Parameter & BoundedGammaDistribution::limit() {
    return _limit;
  }

  
} // namespace Genfun


#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Parameter.h"
#include <cmath>
using namespace Genfun;

class BoundedGammaVisFunction: public VisFunction {
 
public:
 
  BoundedGammaVisFunction():VisFunction("BoundedGamma") {
    

    // PREPARE THE FIT.  THIS IS A TWO-PARAMETER FIT.
    Parameter * pPeak   = new Parameter("Peak",  2.0,  0.1,  10.0);
    Parameter * pWidth  = new Parameter("Width", 1.0,  0.1,  10.0);
    Parameter * pLimit  = new Parameter("Limit", 4.0,  0.0,  10.0);


    addParameter(pPeak);
    addParameter(pWidth);
    addParameter(pLimit);


    // NOW MAKE A FIT FUNCTION AND CONNECT IT TO ITS PARAMETERS:
    Genfun::BoundedGammaDistribution f;

    f.limit().connectFrom(pLimit);
    f.peak().connectFrom(pPeak);
    f.width().connectFrom(pWidth);
    
    PRectF & nr = rectHint();
    nr.setXmin(0);
    nr.setXmax(4);
    nr.setYmin(0);
    nr.setYmax(4);


    addFunction(f);

  }
};


extern "C" BoundedGammaVisFunction *create_BoundedGammaVisFunction()  {
  return new BoundedGammaVisFunction();
}
