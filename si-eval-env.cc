#include "si-eval-env.h"
#include "si-error.h"
#include "si-eval-lambda.h"
#include "si-eval-util.h"
#include <cassert>

namespace si {

bool assignp(Var exp) {
    return taglistp(exp, SET);
}
bool definep(Var exp) {
    return taglistp(exp, DEFINE);
}

Var lookup(Var exp, Var env) {
    auto sb = GET(exp, Symbol);
    while (env) {
        auto &frame = GET(car(env), Frame);
        auto it = frame.find(sb);
        if (it == frame.end()) {
            env = cdr(env);
        } else {
            return it->second;
        }
    }
    unbound(exp);
    return nil;
}
Var extend_env(Var args, Var argl, Var env) {
    env = cons(make(Frame()), env);
    Frame &frame = GET(car(env), Frame);
    while (args && CHECK(args, Pair) && argl) {
        frame[GET(car(args), Symbol)] = car(argl);
        args = cdr(args), argl = cdr(argl);
    }
    if (args != nil) {
        wish(!CHECK(args, Pair), "Too few arguments...", SKIP);
        frame[GET(args, Symbol)] = argl;
    } else {
        wish(argl == nil, "Too many arguments...", SKIP);
    }
    return env;
}

Var assign_var(Var exp) {
    return ncar(exp, 1);
}
Var assign_val(Var exp) {
    return ncar(exp, 2);
}
void set_var_val(Var var, Var val, Var env) {
    auto sb = GET(var, Symbol);
    while (env) {
        auto &frame = GET(car(env), Frame);
        auto it = frame.find(sb);
        if (it == frame.end()) {
            env = cdr(env);
        } else {
            frame[sb] = val;
            return;
        }
    }
    unbound(var);
}

static bool abbrev(Var exp) {
    return CHECK(ncar(exp, 1), Pair);
}
Var define_var(Var exp) {
    if (abbrev(exp)) {
        return car(ncar(exp, 1));
    } else {
        return ncar(exp, 1);
    }
}
Var define_val(Var exp) {
    if (abbrev(exp)) {
        auto args = cdr(ncar(exp, 1));
        auto body = ncdr(exp, 2);
        return make_lambda(args, body);
    } else {
        return ncar(exp, 2);
    }
}
void define_var_val(Var var, Var val, Var env) {
    auto sb = GET(var, Symbol);
    GET(car(env), Frame)[sb] = val;
}
Var make_define(Var var, Var val) {
    return list({getBTSB(DEFINE), var, val});
}

}; // namespace si