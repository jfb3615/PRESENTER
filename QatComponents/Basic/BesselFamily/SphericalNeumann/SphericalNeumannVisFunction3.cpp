#include "SphericalNeumannVisFunction.h"

extern "C" SphericalNeumannVisFunction *create_SphericalNeumannVisFunction3()  {
  return new SphericalNeumannVisFunction(3);
}
