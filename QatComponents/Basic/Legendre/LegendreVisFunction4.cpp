#include "LegendreVisFunction.h"

extern "C" LegendreVisFunction *create_LegendreVisFunction4()  {
  return new LegendreVisFunction(4);
}
