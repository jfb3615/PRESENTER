#include "TchebyshevVisFunction.h"

extern "C" TchebyshevVisFunction *create_TchebyshevVisFunction1()  {
  return new TchebyshevVisFunction(1);
}
