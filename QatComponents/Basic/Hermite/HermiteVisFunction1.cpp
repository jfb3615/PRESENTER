#include "HermiteVisFunction.h"

extern "C" HermiteVisFunction *create_HermiteVisFunction1()  {
  return new HermiteVisFunction(1);
}
