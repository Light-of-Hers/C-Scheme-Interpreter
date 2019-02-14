#ifndef _si_eval_env_h_
#define _si_eval_env_h_

#include "si-value.h"

namespace si {

bool assignp(Var);
bool definep(Var);

Var lookup(Var exp, Var env);
Var extend_env(Var args, Var argl, Var env);

Var assign_var(Var exp);
Var assign_val(Var exp);
void set_var_val(Var var, Var val, Var env);

Var define_var(Var exp);
Var define_val(Var exp);
void define_var_val(Var var, Var val, Var env);
Var make_define(Var var, Var val);

}; // namespace si

#endif