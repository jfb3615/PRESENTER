#include "../HOVisFunction.h"
extern "C" HOVisFunction *create_HO0A1RealVisFunction()  {
  return new HOVisFunction(0,1,HOVisFunction::REAL);
}
