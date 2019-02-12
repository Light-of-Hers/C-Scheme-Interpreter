#ifndef _si_eval_appl_h_
#define _si_eval_appl_h_

#include "si-value.h"

namespace si {

bool applp(Var exp);
Var operand(Var exp);
Var operatr(Var exp);
bool noperandp(Var exp);
Var first_operand(Var exp);
Var rest_operands(Var exp);
bool last_operandp(Var exp);
//  Var adjoin_arg(Var arg, Var argl);

bool comp_procp(Var proc);

Var proc_args(Var proc);
Var proc_body(Var proc);
Var proc_env(Var proc);
Var make_proc(Var args, Var body, Var env);

}; // namespace si

#endif