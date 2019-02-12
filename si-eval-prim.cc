#include "si-eval-prim.h"
#include "si-error.h"
#include "si-eval-env.h"
#include "si-eval-if.h"
#include "si-eval-util.h"
#include "si-number.h"
#include <cassert>
#include <cmath>
#include <functional>
#include <utility>
#include <vector>

namespace si {

typedef std::function<Var(Var)> PProc;
#define PP(body) ([](Var args) -> Var body)

static const std::string msg1 = "Incorrect argument count";
static const std::string msg2 = "Invalid argument";

#define argcnt(pred) wish(length(args) pred, msg1, SKIP)
#define argtype(pred) wish(pred, msg2, SKIP)

Var add(Var args) {
    Number res = 0;
    while (args)
        res += GET(car(args), Number), args = cdr(args);
    return make(res);
}
Var mul(Var args) {
    Number res = 1;
    while (args)
        res *= GET(car(args), Number), args = cdr(args);
    return make(res);
}
Var sub(Var args) {
    argcnt(>= 1);
    Number res = GET(car(args), Number);
    args = cdr(args);
    while (args)
        res -= GET(car(args), Number), args = cdr(args);
    return make(res);
}
Var div(Var args) {
    argcnt(>= 1);
    Number res = GET(car(args), Number);
    args = cdr(args);
    while (args)
        res /= GET(car(args), Number), args = cdr(args);
    return make(res);
}
Var rem(Var args) {
    argcnt(== 2);
    auto a = GET(ncar(args, 0), Number), b = GET(ncar(args, 1), Number);
    argtype(std::floor(a) == a && std::floor(b) == b);
    return make((Number)((int)a % (int)b));
}
Var cons_(Var args) {
    argcnt(== 2);
    return cons(ncar(args, 0), ncar(args, 1));
}
Var car_(Var args) {
    argcnt(== 1);
    return car(car(args));
}
Var cdr_(Var args) {
    argcnt(== 1);
    return cdr(car(args));
}
Var set_car(Var args) {
    argcnt(== 2);
    auto a = car(args), b = ncar(args, 1);
    car(a) = b;
    return getBTSB(OK);
}
Var set_cdr(Var args) {
    argcnt(== 2);
    auto a = car(args), b = ncar(args, 1);
    cdr(a) = b;
    return getBTSB(OK);
}

#define EPS 1e-8
static int ncmp(Number a, Number b) {
    if (a - b > EPS) {
        return 1;
    } else if (b - a > EPS) {
        return -1;
    } else {
        return 0;
    }
}
#define NCMP(op)                                                               \
    argcnt(== 2);                                                              \
    auto a = GET(ncar(args, 0), Number), b = GET(ncar(args, 1), Number);       \
    return getBoolean(ncmp(a, b) op 0);
Var neq(Var args) {
    NCMP(==);
}
Var nl(Var args) {
    NCMP(<);
}
Var ng(Var args) {
    NCMP(>);
}
Var nle(Var args) {
    NCMP(<=);
}
Var nge(Var args) {
    NCMP(>=);
}
Var eq_(Var args) {
    argcnt(== 2);
    auto a = ncar(args, 0), b = ncar(args, 1);
    return getBoolean(eq(a, b));
}
#define checkt(t)                                                              \
    [](Var args) -> Var {                                                      \
        argcnt(== 1);                                                          \
        return getBoolean(CHECK(ncar(args, 0), t));                            \
    }
Var null(Var args) {
    argcnt(== 1);
    return getBoolean(car(args) == nil);
}
Var not_(Var args) {
    argcnt(== 1);
    return getBoolean(!truep(car(args)));
}

static std::vector<std::pair<std::string, PProc>> pps{
    {"+", add},
    {"-", sub},
    {"*", mul},
    {"/", div},
    {"%", rem},
    {"cons", cons_},
    {"car", car_},
    {"cdr", cdr_},
    {"set-car!", set_car},
    {"set-cdr!", set_cdr},
    {"=", neq},
    {"<", nl},
    {">", ng},
    {"<=", nle},
    {">=", nge},
    {"not", not_},
    {"eq?", eq_},
    {"null", null},
    {"string?", checkt(String)},
    {"boolean?", checkt(Boolean)},
    {"number?", checkt(Number)},
    {"symbol?", checkt(Symbol)},
    {"pair?", checkt(Pair)}};

static Var make_pp(int idx) {
    return list({getBTSB(PRIM), make((Number)idx)});
}
bool prim_procp(Var proc) {
    return taglistp(proc, PRIM);
}
Var apply_pp(Var proc, Var argl) {
    int idx = (int)GET(ncar(proc, 1), Number);
    return pps[idx].second(argl);
}
void initPProc(Var env) {
    int len = pps.size();
    for (int i = 0; i < len; ++i) {
        Symbol sb = addSymbol(pps[i].first);
        define_var_val(getSymbol(sb), make_pp(i), env);
    }
}

} // namespace si