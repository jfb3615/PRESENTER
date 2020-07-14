#include "TchebyshevVisFunction.h"

extern "C" TchebyshevVisFunction *create_TchebyshevVisFunction5()  {
  return new TchebyshevVisFunction(5);
}
