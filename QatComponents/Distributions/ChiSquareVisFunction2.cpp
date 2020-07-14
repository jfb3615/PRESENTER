#include "ChiSquareVisFunction.h"

extern "C" ChiSquareVisFunction *create_ChiSquareVisFunction2()  {
  return new ChiSquareVisFunction(2);
}
