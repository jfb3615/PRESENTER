#include "HermiteVisFunction.h"

extern "C" HermiteVisFunction *create_HermiteVisFunction2()  {
  return new HermiteVisFunction(2);
}
