#include "BesselVisFunction.h"

extern "C" BesselVisFunction *create_BesselVisFunction3()  {
  return new BesselVisFunction(3);
}
