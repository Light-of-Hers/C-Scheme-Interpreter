#include "si-eval-if.h"
#include "si-eval-util.h"

namespace si {

bool ifp(Var exp) {
    return taglistp(exp, IF);
}
Var if_pred(Var exp) {
    return ncar(exp, 1);
}
Var if_then(Var exp) {
    return ncar(exp, 2);
}
Var if_else(Var exp) {
    return ncar(exp, 3);
}
bool falsep(Var exp) {
    return CHECK(exp, Boolean) && GET(exp, Boolean) == false;
}
bool truep(Var exp) {
    return !falsep(exp);
}

}; // namespace si