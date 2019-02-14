#include "si-eval-util.h"
#include "si-io.h"
#include "si-number.h"

namespace si {

bool self_evalp(Var exp) {
    return CHECK(exp, String) || CHECK(exp, Number) || CHECK(exp, Boolean) ||
           eq(exp, getBTSB(UNASS));
}
bool variablep(Var exp) {
    return CHECK(exp, Symbol);
}
bool quotedp(Var exp) {
    return taglistp(exp, QUOTE);
}
Var quote_text(Var exp) {
    return ncar(exp, 1);
}

Var ncar(Var lst, int n) {
    while (n--)
        lst = cdr(lst);
    return car(lst);
}
Var ncdr(Var lst, int n) {
    while (n--)
        lst = cdr(lst);
    return lst;
}
bool eq(Var a, Var b) {
    return a == b;
}
Var list(std::vector<Var> vars) {
    Var head = guard;
    Var cur = head;
    for (auto v : vars)
        cur = cdr(cur) = cons(v, nil);
    return cdr(head);
}
bool taglistp_(Var lst, Symbol tag) {
    return CHECK(lst, Pair) && CHECK(car(lst), Symbol) &&
           GET(car(lst), Symbol) == tag;
}
int length(Var lst) {
    int n = 0;
    while (lst)
        n++, lst = cdr(lst);
    return n;
}
Var map(Var lst, std::function<Var(Var)> proc) {
    Var head = guard;
    Var cur = head;
    while (lst) {
        cur = cdr(cur) = cons(proc(car(lst)), nil);
        lst = cdr(lst);
    }
    return cdr(head);
}

}; // namespace si