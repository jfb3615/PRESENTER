#include "BesselVisFunction.h"

extern "C" BesselVisFunction *create_BesselVisFunction5()  {
  return new BesselVisFunction(5);
}
