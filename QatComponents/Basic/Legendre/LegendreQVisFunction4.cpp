#include "LegendreQVisFunction.h"

extern "C" LegendreQVisFunction *create_LegendreQVisFunction4()  {
  return new LegendreQVisFunction(4);
}
