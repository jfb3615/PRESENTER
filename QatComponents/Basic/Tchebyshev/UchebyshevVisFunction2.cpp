#include "UchebyshevVisFunction.h"

extern "C" UchebyshevVisFunction *create_UchebyshevVisFunction2()  {
  return new UchebyshevVisFunction(2);
}
