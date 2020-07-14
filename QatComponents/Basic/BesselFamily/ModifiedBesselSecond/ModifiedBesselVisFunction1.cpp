#include "ModifiedBesselVisFunction.h"

extern "C" ModifiedBesselVisFunction *create_ModifiedBesselVisFunction1()  {
  return new ModifiedBesselVisFunction(1);
}
