#include "si-eval-if.h"
#include "si-eval-begin.h"
#include "si-eval-lambda.h"
#include "si-eval-util.h"

namespace si {

bool ifp(Var exp) {
    return taglistp(exp, IF);
}
Var if_pred(Var exp) {
    return ncar(exp, 1);
}
Var if_then(Var exp) {
    return ncar(exp, 2);
}
Var if_else(Var exp) {
    return ncar(exp, 3);
}
Var make_if(Var pred, Var then, Var alter) {
    return list({getBTSB(IF), pred, then, alter});
}
bool falsep(Var exp) {
    return CHECK(exp, Boolean) && GET(exp, Boolean) == false;
}
bool truep(Var exp) {
    return !falsep(exp);
}
bool andp(Var exp) {
    return taglistp(exp, AND);
}
static Var and2if_aux(Var preds) {
    if (preds == nil) {
        return getBoolean(true);
    } else if (cdr(preds) == nil) {
        return car(preds);
    } else {
        return make_if(car(preds), and2if_aux(cdr(preds)), getBoolean(false));
    }
}
Var and2if(Var exp) {
    return and2if_aux(cdr(exp));
}

bool orp(Var exp) {
    return taglistp(exp, OR);
}
static Var or2if_aux(Var preds) {
    if (preds == nil) {
        return getBoolean(false);
    } else {
        auto or_test = getBTSB(OR_TEST);
        return make_1let(
            or_test, car(preds),
            list({make_if(or_test, or_test, or2if_aux(cdr(preds)))}));
    }
}
Var or2if(Var exp) {
    return or2if_aux(cdr(exp));
}

bool condp(Var exp) {
    return taglistp(exp, COND);
}
static bool arrowp(Var cls) {
    return eq(ncar(cls, 1), getBTSB(ARROW));
}
static Var cond2if_aux(Var clss) {
    if (clss == nil)
        return getBoolean(false);
    auto cls = car(clss);
    auto pred = car(cls);
    if (eq(pred, getBTSB(ELSE)))
        wish(cdr(clss) == nil, "Else's not last", SKIP);
    if (arrowp(cls)) {
        auto proc = ncar(cls, 2);
        auto cond_test = getBTSB(COND_TEST);
        return make_1let(cond_test, pred,
                         list({make_if(cond_test, list({proc, cond_test}),
                                       cond2if_aux(cdr(clss)))}));
    } else {
        auto actions = ncdr(cls, 1);
        return make_if(pred, make_begin(actions), cond2if_aux(cdr(clss)));
    }
}
Var cond2if(Var exp) {
    return cond2if_aux(cdr(exp));
}

}; // namespace si