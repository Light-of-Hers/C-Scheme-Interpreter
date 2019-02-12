#ifndef _si_io_h_
#define _si_io_h_

#include "si-value.h"
#include <iostream>

namespace si {

std::istream &operator>>(std::istream &is, Var &x);
std::ostream &operator<<(std::ostream &os, Var x);

}; // namespace si

#endif