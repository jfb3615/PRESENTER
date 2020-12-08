#include "LegendreQVisFunction.h"

extern "C" LegendreQVisFunction *create_LegendreQVisFunction2()  {
  return new LegendreQVisFunction(2);
}
