#include "si-number.h"

namespace si {

Number ston(const String &s) {
    return std::stod(s);
}

}; // namespace si