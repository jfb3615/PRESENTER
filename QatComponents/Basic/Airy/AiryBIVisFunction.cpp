#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Airy.h"
#include <cmath>
using namespace Genfun;

class AiryBIVisFunction: public VisFunction {
 
public:
 
  AiryBIVisFunction():VisFunction("Airy Bi") {


    Airy airy(Airy::Bi);

    PRectF & nr =  rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(-1);
    nr.setYmax(1);

    addFunction(airy);

  }
};


extern "C" AiryBIVisFunction *create_AiryBIVisFunction()  {
  return new AiryBIVisFunction();
}
