#include "QatPlotting/VisFunction.h"
#include "QatGenericFunctions/Airy.h"          
#include <cmath>
using namespace Genfun;

class AiryAIVisFunction: public VisFunction {
 
public:
 
  AiryAIVisFunction():VisFunction("Airy Ai") {


    Airy airy(Airy::Ai);

    PRectF  & nr = rectHint();
    nr.setXmin(-10);
    nr.setXmax(10);
    nr.setYmin(-1);
    nr.setYmax(1);

    addFunction(airy);

  }
};


extern "C" AiryAIVisFunction *create_AiryAIVisFunction()  {
  return new AiryAIVisFunction();
}
