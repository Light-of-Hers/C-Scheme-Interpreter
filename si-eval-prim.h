#ifndef _si_eval_prim_h_
#define _si_eval_prim_h_

#include "si-value.h"

namespace si {

void initPProc(Var env);
bool prim_procp(Var proc);
Var apply_pp(Var proc, Var argl);

} // namespace si

#endif