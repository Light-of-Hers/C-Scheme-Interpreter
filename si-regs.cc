#include "si-regs.h"
#include <vector>

namespace si {

#define SIZE 7
static Var regs[SIZE];

void initRegs() {
    for (int i = 0; i < SIZE; ++i)
        regs[i] = nullptr;
}

Var &getReg(Reg r) {
    return regs[(int)r];
}
void setReg(Reg r, Var x) {
    regs[(int)r] = x;
}

}; // namespace si