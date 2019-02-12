#include "si-eval-appl.h"
#include "si-eval-util.h"

namespace si {

bool applp(Var exp) {
    return CHECK(exp, Pair);
};
Var operand(Var exp) {
    return cdr(exp);
}
Var operatr(Var exp) {
    return car(exp);
}
bool noperandp(Var exp) {
    return exp == nil;
}
Var first_operand(Var exp) {
    return car(exp);
}
Var rest_operands(Var exp) {
    return cdr(exp);
}
bool last_operandp(Var exp) {
    return cdr(exp) == nil;
}
bool comp_procp(Var proc) {
    return taglistp(proc, PROC);
}
Var proc_args(Var proc) {
    return ncar(proc, 1);
}
Var proc_body(Var proc) {
    return ncar(proc, 2);
}
Var proc_env(Var proc) {
    return ncar(proc, 3);
}
Var make_proc(Var args, Var body, Var env) {
    return list({getBTSB(PROC), args, body, env});
}

}; // namespace si