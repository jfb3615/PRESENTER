#include "TchebyshevVisFunction.h"

extern "C" TchebyshevVisFunction *create_TchebyshevVisFunction4()  {
  return new TchebyshevVisFunction(4);
}
