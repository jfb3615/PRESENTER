#include "../HOVisFunction.h"
extern "C" HOVisFunction *create_HO1RealVisFunction()  {
  return new HOVisFunction(1,HOVisFunction::REAL);
}
