#include "SphericalBesselVisFunction.h"

extern "C" SphericalBesselVisFunction *create_SphericalBesselVisFunction3()  {
  return new SphericalBesselVisFunction(3);
}
