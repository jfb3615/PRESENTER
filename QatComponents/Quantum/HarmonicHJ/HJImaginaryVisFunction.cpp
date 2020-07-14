#include "HJVisFunction.h"
extern "C" HJVisFunction *create_HJImaginaryVisFunction()  {
  return new HJVisFunction(HJVisFunction::IMAGINARY);
}
