#include "NeumannVisFunction.h"

extern "C" NeumannVisFunction *create_NeumannVisFunction1()  {
  return new NeumannVisFunction(1);
}
