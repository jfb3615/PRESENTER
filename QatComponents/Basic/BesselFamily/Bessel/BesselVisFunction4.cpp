#include "BesselVisFunction.h"

extern "C" BesselVisFunction *create_BesselVisFunction4()  {
  return new BesselVisFunction(4);
}
