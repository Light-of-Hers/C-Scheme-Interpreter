#ifndef _si_eval_lambda_h_
#define _si_eval_lambda_h_

#include "si-value.h"

namespace si {

bool lambdap(Var);
Var lambda_args(Var exp);
Var lambda_body(Var exp);
Var make_lambda(Var args, Var body);
bool letp(Var);
Var let2exp(Var);
bool let_p(Var);
Var let_2lets(Var);
Var make_1let(Var var, Var val, Var body);
Var make_lets(Var binds, Var body);

}; // namespace si

#endif