#include "LegendreVisFunction.h"

extern "C" LegendreVisFunction *create_LegendreVisFunction3()  {
  return new LegendreVisFunction(3);
}
