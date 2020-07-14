#include "ChiSquareVisFunction.h"

extern "C" ChiSquareVisFunction *create_ChiSquareVisFunction1()  {
  return new ChiSquareVisFunction(1);
}
