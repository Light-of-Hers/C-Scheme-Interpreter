#include "si-eval.h"
#include "si-error.h"
#include "si-eval-appl.h"
#include "si-eval-begin.h"
#include "si-eval-env.h"
#include "si-eval-if.h"
#include "si-eval-lambda.h"
#include "si-eval-prim.h"
#include "si-eval-util.h"
#include "si-global.h"
#include "si-regs.h"
#include "si-stack.h"
#include <cassert>

namespace si {

static void *cont;
static int flag;

#define assign(r, v) setReg(Reg::r, v)
#define reg(r) getReg(Reg::r)
#define test(p) (flag = (p))
#define perform(a) (a)
#define branch(l)                                                              \
    if (flag)                                                                  \
        goto l;
#define jump(l) goto l;
#define ok getBTSB(OK)

void eval_machine() {
    flag = 0;
    cont = &&halt;
    goto eval_dispatch;

unknown_exp_type:
    wish(false, "Unknown expression type", SKIP);

unknown_proc_type:
    wish(false, "Unknown expression type", SKIP);

halt:
    return;

eval_dispatch:
    test(self_evalp(reg(exp)));
    branch(ev_self_eval);
    test(variablep(reg(exp)));
    branch(ev_variable);
    test(quotedp(reg(exp)));
    branch(ev_quoted);
    test(assignp(reg(exp)));
    branch(ev_assign);
    test(definep(reg(exp)));
    branch(ev_define);
    test(ifp(reg(exp)));
    branch(ev_if);
    test(lambdap(reg(exp)));
    branch(ev_lambda);
    test(beginp(reg(exp)));
    branch(ev_begin);
    test(applp(reg(exp)));
    branch(ev_application);
    jump(unknown_exp_type);

ev_self_eval:
    assign(val, reg(exp));
    jump(*cont);

ev_variable:
    assign(val, lookup(reg(exp), reg(env)));
    jump(*cont);

ev_quoted:
    assign(val, quote_text(reg(exp)));
    jump(*cont);

ev_lambda:
    assign(unev, lambda_args(reg(exp)));
    assign(exp, lambda_body(reg(exp)));
    assign(val, make_proc(reg(unev), reg(exp), reg(env)));
    jump(*cont);

ev_application:
    push(cont);
    push(reg(env));
    assign(unev, operand(reg(exp)));
    push(reg(unev));
    assign(exp, operatr(reg(exp)));
    cont = &&ev_appl_operator;
    jump(eval_dispatch);

ev_appl_operator:
    pop(reg(unev));
    pop(reg(env));
    assign(argl, nil);
    assign(proc, reg(val));
    test(noperandp(reg(unev)));
    branch(apply_dispatch);

    push(reg(proc));
    assign(argl, cons(nil, nil));
    assign(tmp, reg(argl));
    push(reg(argl));

ev_appl_operand_loop:
    push(reg(tmp));
    assign(exp, first_operand(reg(unev)));
    test(last_operandp(reg(unev)));
    branch(ev_appl_last_arg);
    push(reg(env));
    push(reg(unev));
    cont = &&ev_appl_acc_arg;
    jump(eval_dispatch);

ev_appl_acc_arg:
    pop(reg(unev));
    pop(reg(env));
    pop(reg(tmp));
    assign(tmp, cdr(reg(tmp)) = cons(reg(val), nil));
    assign(unev, rest_operands(reg(unev)));
    jump(ev_appl_operand_loop);

ev_appl_last_arg:
    cont = &&ev_appl_acc_last_arg;
    jump(eval_dispatch);

ev_appl_acc_last_arg:
    pop(reg(tmp));
    assign(tmp, cdr(reg(tmp)) = cons(reg(val), nil));
    pop(reg(argl));
    pop(reg(proc));
    assign(argl, cdr(reg(argl)));
    jump(apply_dispatch);

apply_dispatch:
    test(prim_procp(reg(proc)));
    branch(primitive_apply);
    test(comp_procp(reg(proc)));
    branch(compound_apply);
    jump(unknown_proc_type);

primitive_apply:
    assign(val, apply_pp(reg(proc), reg(argl)));
    pop(cont);
    jump(*cont);

compound_apply:
    assign(unev, proc_args(reg(proc)));
    assign(env, proc_env(reg(proc)));
    assign(env, extend_env(reg(unev), reg(argl), reg(env)));
    assign(unev, proc_body(reg(proc)));
    jump(ev_sequence);

ev_begin:
    assign(unev, begin_actions(reg(exp)));
    test(reg(unev) != nil);
    branch(ev_begin_doit);
    assign(val, ok);
    jump(*cont);

ev_begin_doit:
    push(cont);
    jump(ev_sequence);

ev_sequence:
    assign(exp, first_exp(reg(unev)));
    test(last_expp(reg(unev)));
    branch(ev_seq_last_exp);
    push(reg(unev));
    push(reg(env));
    cont = &&ev_seq_cont;
    jump(eval_dispatch);

ev_seq_cont:
    pop(reg(env));
    pop(reg(unev));
    assign(unev, rest_exps(reg(unev)));
    jump(ev_sequence);

ev_seq_last_exp:
    pop(cont);
    jump(eval_dispatch);

ev_if:
    push(reg(exp));
    push(reg(env));
    push(cont);
    cont = &&ev_if_decide;
    assign(exp, if_pred(reg(exp)));
    jump(eval_dispatch);

ev_if_decide:
    pop(cont);
    pop(reg(env));
    pop(reg(exp));
    test(truep(reg(val)));
    branch(ev_if_then);

    assign(exp, if_else(reg(exp)));
    jump(eval_dispatch);
ev_if_then:
    assign(exp, if_then(reg(exp)));
    jump(eval_dispatch);

ev_assign:
    assign(unev, assign_var(reg(exp)));
    assign(exp, assign_val(reg(exp)));
    push(reg(unev));
    push(reg(env));
    push(cont);
    cont = &&ev_assign_cont;
    jump(eval_dispatch);

ev_assign_cont:
    pop(cont);
    pop(reg(env));
    pop(reg(unev));
    perform(set_var_val(reg(unev), reg(val), reg(env)));
    assign(val, ok);
    jump(*cont);

ev_define:
    assign(unev, define_var(reg(exp)));
    assign(exp, define_val(reg(exp)));
    push(reg(unev));
    push(reg(env));
    push(cont);
    cont = &&ev_define_cont;
    jump(eval_dispatch);

ev_define_cont:
    pop(cont);
    pop(reg(env));
    pop(reg(unev));
    perform(define_var_val(reg(unev), reg(val), reg(env)));
    assign(val, ok);
    jump(*cont);
}

Var eval(Var exp, Var env) {
    initStack();
    initRegs();
    assign(exp, exp);
    assign(env, env);
    eval_machine();
    initStack();
    return reg(val);
}

} // namespace si