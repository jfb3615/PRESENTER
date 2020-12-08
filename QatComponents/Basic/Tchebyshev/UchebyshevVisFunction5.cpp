#include "UchebyshevVisFunction.h"

extern "C" UchebyshevVisFunction *create_UchebyshevVisFunction5()  {
  return new UchebyshevVisFunction(5);
}
