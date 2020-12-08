#include "LegendreQVisFunction.h"

extern "C" LegendreQVisFunction *create_LegendreQVisFunction3()  {
  return new LegendreQVisFunction(3);
}
