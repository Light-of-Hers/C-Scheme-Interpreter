#include "si-eval-begin.h"
#include "si-eval-util.h"

namespace si {

bool beginp(Var exp) {
    return taglistp(exp, BEGIN);
}
Var begin_actions(Var exp) {
    return cdr(exp);
}
Var first_exp(Var exp) {
    return car(exp);
}
Var rest_exps(Var exp) {
    return cdr(exp);
}
bool last_expp(Var exp) {
    return cdr(exp) == nil;
}
Var make_begin(Var exps) {
    return cons(getBTSB(BEGIN), exps);
}

}; // namespace si