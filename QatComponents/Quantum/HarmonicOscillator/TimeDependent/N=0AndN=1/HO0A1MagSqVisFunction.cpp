#include "../HOVisFunction.h"
extern "C" HOVisFunction *create_HO0A1MagSqVisFunction()  {
  return new HOVisFunction(0,1,HOVisFunction::MAGSQ);
}
