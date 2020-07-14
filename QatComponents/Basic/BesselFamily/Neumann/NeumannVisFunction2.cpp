#include "NeumannVisFunction.h"

extern "C" NeumannVisFunction *create_NeumannVisFunction2()  {
  return new NeumannVisFunction(2);
}
