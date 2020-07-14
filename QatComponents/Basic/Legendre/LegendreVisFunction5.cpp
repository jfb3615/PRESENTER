#include "LegendreVisFunction.h"

extern "C" LegendreVisFunction *create_LegendreVisFunction5()  {
  return new LegendreVisFunction(5);
}
