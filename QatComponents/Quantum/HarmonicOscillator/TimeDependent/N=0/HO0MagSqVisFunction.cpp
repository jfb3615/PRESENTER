#include "../HOVisFunction.h"
extern "C" HOVisFunction *create_HO0MagSqVisFunction()  {
  return new HOVisFunction(0,HOVisFunction::MAGSQ);
}
