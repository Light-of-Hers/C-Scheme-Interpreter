#include "si-eval-lambda.h"
#include "si-eval-begin.h"
#include "si-eval-env.h"
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

bool letp(Var exp) {
    return taglistp(exp, LET);
}
static bool named_letp(Var exp) {
    return CHECK(ncar(exp, 1), Symbol);
}
Var let2exp(Var exp) {
    if (named_letp(exp)) {
        auto name = ncar(exp, 1);
        auto binds = ncar(exp, 2);
        auto body = ncdr(exp, 3);
        auto vars = map(binds, car);
        auto vals = map(binds, [](Var x) { return ncar(x, 1); });
        return make_begin(list(
            {make_define(name, make_lambda(vars, body)), cons(name, vals)}));
    } else {
        auto binds = ncar(exp, 1);
        auto body = ncdr(exp, 2);
        if (binds == nil)
            return make_begin(body);
        auto vars = map(binds, car);
        auto vals = map(binds, [](Var x) { return ncar(x, 1); });
        return cons(make_lambda(vars, body), vals);
    }
}
Var make_lets(Var binds, Var body) {
    return cons(getBTSB(LET), cons(binds, body));
}
Var make_1let(Var var, Var val, Var body) {
    auto binds = list({list({var, val})});
    return make_lets(binds, body);
}

bool let_p(Var exp) {
    return taglistp(exp, LET_);
}
static Var let_2lets_aux(Var binds, Var body) {
    if (binds == nil) {
        return make_begin(body);
    } else if (cdr(binds) == nil) {
        return make_lets(binds, body);
    } else {
        auto var = ncar(car(binds), 0);
        auto val = ncar(car(binds), 1);
        return make_1let(var, val, list({let_2lets_aux(cdr(binds), body)}));
    }
}
Var let_2lets(Var exp) {
    auto binds = ncar(exp, 1);
    auto body = ncdr(exp, 2);
    return let_2lets_aux(binds, body);
}

}; // namespace si