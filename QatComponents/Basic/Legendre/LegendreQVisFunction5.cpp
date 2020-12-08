#include "LegendreQVisFunction.h"

extern "C" LegendreQVisFunction *create_LegendreQVisFunction5()  {
  return new LegendreQVisFunction(5);
}
