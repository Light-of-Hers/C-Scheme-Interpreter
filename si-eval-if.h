#ifndef _si_eval_if_h_
#define _si_eval_if_h_

#include "si-value.h"

namespace si {

bool ifp(Var);
Var if_pred(Var exp);
Var if_then(Var exp);
Var if_else(Var exp);
Var make_if(Var pred, Var then, Var alter);
bool truep(Var exp);
bool falsep(Var exp);
bool andp(Var exp);
Var and2if(Var exp);
bool orp(Var exp);
Var or2if(Var exp);
bool condp(Var exp);
Var cond2if(Var exp);

}; // namespace si

#endif