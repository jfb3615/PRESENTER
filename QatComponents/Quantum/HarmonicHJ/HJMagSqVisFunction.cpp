#include "HJVisFunction.h"
extern "C" HJVisFunction *create_HJMagSqVisFunction()  {
  return new HJVisFunction(HJVisFunction::MAGSQ);
}
