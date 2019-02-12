#ifndef _si_regs_h_
#define _si_regs_h_

#include "si-value.h"

namespace si {

enum class Reg { exp = 0, env, val, proc, argl, unev, tmp };

void initRegs();
Var& getReg(Reg r);
void setReg(Reg r, Var x);

}; // namespace si

#endif