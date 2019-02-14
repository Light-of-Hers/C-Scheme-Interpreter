#ifndef _si_eval_begin_h_
#define _si_eval_begin_h_

#include "si-value.h"

namespace si {

bool beginp(Var);
Var begin_actions(Var exp);
Var first_exp(Var exp);
Var rest_exps(Var exp);
bool last_expp(Var exp);
Var make_begin(Var exps);

}; // namespace si

#endif