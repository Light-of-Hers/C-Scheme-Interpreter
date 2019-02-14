#ifndef _si_eval_util_h_
#define _si_eval_util_h_

#include "si-global.h"
#include "si-value.h"
#include <functional>
#include <vector>

namespace si {

bool self_evalp(Var);
bool variablep(Var);
bool quotedp(Var);
Var quote_text(Var exp);
Var ncar(Var lst, int n);
Var ncdr(Var lst, int n);
bool eq(Var a, Var b);
Var list(std::vector<Var> vars);
int length(Var lst);
bool taglistp_(Var lst, Symbol tag);
#define taglistp(lst, tag) taglistp_(lst, (Symbol)BTSB::tag)
Var map(Var lst, std::function<Var(Var)>);

}; // namespace si

#endif