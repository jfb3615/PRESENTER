#include "ChiSquareVisFunction.h"

extern "C" ChiSquareVisFunction *create_ChiSquareVisFunction3()  {
  return new ChiSquareVisFunction(3);
}
