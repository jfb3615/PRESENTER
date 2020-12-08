#include "UchebyshevVisFunction.h"

extern "C" UchebyshevVisFunction *create_UchebyshevVisFunction1()  {
  return new UchebyshevVisFunction(1);
}
