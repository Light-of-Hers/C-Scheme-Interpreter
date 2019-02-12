#ifndef _si_eval_if_h_
#define _si_eval_if_h_

#include "si-value.h"

namespace si {

bool ifp(Var);
Var if_pred(Var exp);
Var if_then(Var exp);
Var if_else(Var exp);
bool truep(Var exp);
bool falsep(Var exp);

}; // namespace si

#endif