#ifndef _LegendreQVisFunction_h_
#define _LegendreQVisFunction_h_

#include "QatPlotting/VisFunction.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/FixedConstant.h"
#include "QatGenericFunctions/F1D.h"          
#include <cmath>
#include <sstream>
#include "gsl/gsl_sf_legendre.h"
using namespace Genfun;

inline std::string toString(int i) {
  std::ostringstream o;
  o << i;
  return o.str();
}

template <int l> double legendreQ(double x) {
  if (l<0 || l>5) throw std::runtime_error("Vis function for l<0 or l>5 not implemented");
  return gsl_sf_legendre_Ql(l,x);
}
class LegendreQVisFunction: public VisFunction {
 
public:
 
  LegendreQVisFunction(int l):VisFunction("Q_"+toString(l)) {

    F1D p=   l==0 ? legendreQ<0> :
      l==1 ? legendreQ<1> :
      l==2 ? legendreQ<2> :
      l==3 ? legendreQ<3> :
      l==4 ? legendreQ<4> :
      l==5 ? legendreQ<5> : legendreQ<6>; // last case is an error.  
    
    const Cut<double> & cut = RealArg::Gt(-1) && RealArg::Lt(1);
    
    PRectF & nr = rectHint();
    nr.setXmin(-1);
    nr.setXmax(1);
    nr.setYmin(-5);
    nr.setYmax(5);

    addFunction(p,&cut);

  }
};
#endif

