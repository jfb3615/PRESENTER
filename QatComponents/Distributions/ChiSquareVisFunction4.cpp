#include "ChiSquareVisFunction.h"

extern "C" ChiSquareVisFunction *create_ChiSquareVisFunction4()  {
  return new ChiSquareVisFunction(4);
}
