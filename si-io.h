#ifndef _si_io_h_
#define _si_io_h_

#include "si-value.h"
#include <iostream>

namespace si {

std::istream &operator>>(std::istream &is, Var &x);
std::ostream &operator<<(std::ostream &os, Var x);

#ifndef NDEBUG
#define debug(x) (std::cerr << x << std::endl)
#else
#define debug(x) (void)0
#endif
}; // namespace si

#endif