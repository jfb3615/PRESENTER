#include "SphericalNeumannVisFunction.h"

extern "C" SphericalNeumannVisFunction *create_SphericalNeumannVisFunction1()  {
  return new SphericalNeumannVisFunction(1);
}
