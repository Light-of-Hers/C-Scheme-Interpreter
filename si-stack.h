#ifndef _si_stack_h_
#define _si_stack_h_

#include "si-value.h"

namespace si {

void initStack();
void push(Var x);
void push(void *cont);
void pop(Var &x);
void pop(void *&cont);

}; // namespace si

#endif