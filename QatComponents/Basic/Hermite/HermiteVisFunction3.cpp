#include "HermiteVisFunction.h"

extern "C" HermiteVisFunction *create_HermiteVisFunction3()  {
  return new HermiteVisFunction(3);
}
