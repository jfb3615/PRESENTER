#include "SphericalBesselVisFunction.h"

extern "C" SphericalBesselVisFunction *create_SphericalBesselVisFunction5()  {
  return new SphericalBesselVisFunction(5);
}
