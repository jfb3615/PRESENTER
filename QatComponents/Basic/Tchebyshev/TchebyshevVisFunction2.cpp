#include "TchebyshevVisFunction.h"

extern "C" TchebyshevVisFunction *create_TchebyshevVisFunction2()  {
  return new TchebyshevVisFunction(2);
}
