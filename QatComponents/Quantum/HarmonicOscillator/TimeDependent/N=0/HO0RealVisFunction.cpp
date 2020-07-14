#include "../HOVisFunction.h"
extern "C" HOVisFunction *create_HO0RealVisFunction()  {
  return new HOVisFunction(0,HOVisFunction::REAL);
}
