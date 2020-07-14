#include "NeumannVisFunction.h"

extern "C" NeumannVisFunction *create_NeumannVisFunction4()  {
  return new NeumannVisFunction(4);
}
