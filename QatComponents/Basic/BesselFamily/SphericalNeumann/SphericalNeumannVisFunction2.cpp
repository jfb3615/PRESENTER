#include "SphericalNeumannVisFunction.h"

extern "C" SphericalNeumannVisFunction *create_SphericalNeumannVisFunction2()  {
  return new SphericalNeumannVisFunction(2);
}
