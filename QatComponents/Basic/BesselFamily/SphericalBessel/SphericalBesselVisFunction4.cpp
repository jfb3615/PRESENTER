#include "SphericalBesselVisFunction.h"

extern "C" SphericalBesselVisFunction *create_SphericalBesselVisFunction4()  {
  return new SphericalBesselVisFunction(4);
}
