#include "BesselVisFunction.h"

extern "C" BesselVisFunction *create_BesselVisFunction2()  {
  return new BesselVisFunction(2);
}
