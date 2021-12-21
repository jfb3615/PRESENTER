#include "SphericalNeumannVisFunction.h"

extern "C" SphericalNeumannVisFunction *create_SphericalNeumannVisFunction5()  {
  return new SphericalNeumannVisFunction(5);
}
