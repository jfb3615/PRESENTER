#include "SphericalBesselVisFunction.h"

extern "C" SphericalBesselVisFunction *create_SphericalBesselVisFunction2()  {
  return new SphericalBesselVisFunction(2);
}
