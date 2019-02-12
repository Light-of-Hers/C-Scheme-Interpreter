#ifndef _si_eval_h_
#define _si_eval_h_

#include "si-value.h"

namespace si {
Var eval(Var exp, Var env);
};

#endif