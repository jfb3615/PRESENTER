#include "../HOVisFunction.h"
extern "C" HOVisFunction *create_HO1MagSqVisFunction()  {
  return new HOVisFunction(1,HOVisFunction::MAGSQ);
}
