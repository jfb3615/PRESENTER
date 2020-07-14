#include "ChiSquareVisFunction.h"

extern "C" ChiSquareVisFunction *create_ChiSquareVisFunction8()  {
  return new ChiSquareVisFunction(8);
}
