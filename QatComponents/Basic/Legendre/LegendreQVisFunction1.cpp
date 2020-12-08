#include "LegendreQVisFunction.h"

extern "C" LegendreQVisFunction *create_LegendreQVisFunction1()  {
  return new LegendreQVisFunction(1);
}
