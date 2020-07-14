#include "ModifiedBesselVisFunction.h"

extern "C" ModifiedBesselVisFunction *create_ModifiedBesselVisFunction3()  {
  return new ModifiedBesselVisFunction(3);
}
