#include "NeumannVisFunction.h"

extern "C" NeumannVisFunction *create_NeumannVisFunction5()  {
  return new NeumannVisFunction(5);
}
