#include "SphericalNeumannVisFunction.h"

extern "C" SphericalNeumannVisFunction *create_SphericalNeumannVisFunction4()  {
  return new SphericalNeumannVisFunction(4);
}
