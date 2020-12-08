#include "UchebyshevVisFunction.h"

extern "C" UchebyshevVisFunction *create_UchebyshevVisFunction4()  {
  return new UchebyshevVisFunction(4);
}
