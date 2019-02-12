#ifndef _si_eval_lambda_h_
#define _si_eval_lambda_h_

#include "si-value.h"

namespace si {

bool lambdap(Var);
Var lambda_args(Var exp);
Var lambda_body(Var exp);
Var make_lambda(Var args, Var body);


}; // namespace si

#endif