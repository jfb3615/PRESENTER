#include "ChiSquareVisFunction.h"

extern "C" ChiSquareVisFunction *create_ChiSquareVisFunction5()  {
  return new ChiSquareVisFunction(5);
}
