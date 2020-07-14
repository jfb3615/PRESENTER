#include "HermiteVisFunction.h"

extern "C" HermiteVisFunction *create_HermiteVisFunction5()  {
  return new HermiteVisFunction(5);
}
