#include "si-eval-lambda.h"
#include "si-eval-util.h"

namespace si {

bool lambdap(Var exp) {
    return taglistp(exp, LAMBDA);
}
Var lambda_args(Var exp) {
    return ncar(exp, 1);
}
Var lambda_body(Var exp) {
    return ncdr(exp, 2);
}
Var make_lambda(Var args, Var body) {
    return cons(getBTSB(LAMBDA), cons(args, body));
}

}; // namespace si