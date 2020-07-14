#include "LegendreVisFunction.h"

extern "C" LegendreVisFunction *create_LegendreVisFunction2()  {
  return new LegendreVisFunction(2);
}
