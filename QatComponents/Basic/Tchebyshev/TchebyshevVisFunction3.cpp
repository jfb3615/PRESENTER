#include "TchebyshevVisFunction.h"

extern "C" TchebyshevVisFunction *create_TchebyshevVisFunction3()  {
  return new TchebyshevVisFunction(3);
}
