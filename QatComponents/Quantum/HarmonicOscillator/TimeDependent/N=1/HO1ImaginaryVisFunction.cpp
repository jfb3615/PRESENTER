#include "../HOVisFunction.h"
extern "C" HOVisFunction *create_HO1ImaginaryVisFunction()  {
  return new HOVisFunction(1,HOVisFunction::IMAGINARY);
}
