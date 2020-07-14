#include "HJVisFunction.h"
extern "C" HJVisFunction *create_HJRealVisFunction()  {
  return new HJVisFunction(HJVisFunction::REAL);
}
