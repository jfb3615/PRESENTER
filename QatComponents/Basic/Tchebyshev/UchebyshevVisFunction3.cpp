#include "UchebyshevVisFunction.h"

extern "C" UchebyshevVisFunction *create_UchebyshevVisFunction3()  {
  return new UchebyshevVisFunction(3);
}
