#include "../HOVisFunction.h"
extern "C" HOVisFunction *create_HO0ImaginaryVisFunction()  {
  return new HOVisFunction(0,HOVisFunction::IMAGINARY);
}
