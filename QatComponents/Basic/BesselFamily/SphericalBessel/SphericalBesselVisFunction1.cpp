#include "SphericalBesselVisFunction.h"

extern "C" SphericalBesselVisFunction *create_SphericalBesselVisFunction1()  {
  return new SphericalBesselVisFunction(1);
}
