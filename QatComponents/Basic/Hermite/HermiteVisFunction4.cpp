#include "HermiteVisFunction.h"

extern "C" HermiteVisFunction *create_HermiteVisFunction4()  {
  return new HermiteVisFunction(4);
}
