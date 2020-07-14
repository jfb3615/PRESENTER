#include "LegendreVisFunction.h"

extern "C" LegendreVisFunction *create_LegendreVisFunction1()  {
  return new LegendreVisFunction(1);
}
