#include "BesselVisFunction.h"

extern "C" BesselVisFunction *create_BesselVisFunction1()  {
  return new BesselVisFunction(1);
}
