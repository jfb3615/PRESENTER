#include "NeumannVisFunction.h"

extern "C" NeumannVisFunction *create_NeumannVisFunction3()  {
  return new NeumannVisFunction(3);
}
