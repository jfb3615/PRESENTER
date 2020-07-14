#include "ModifiedBesselVisFunction.h"

extern "C" ModifiedBesselVisFunction *create_ModifiedBesselVisFunction2()  {
  return new ModifiedBesselVisFunction(2);
}
