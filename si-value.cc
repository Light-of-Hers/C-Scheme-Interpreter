#include "si-value.h"
#include "si-error.h"
#include "si-global.h"
#include "si-number.h"
#include <sstream>

namespace si {

Var cons(Var a, Var b) {
    return std::make_shared<Value>(Pair({a, b}));
}
Var &car(Var x) {
    return GET(x, Pair).chld[0];
}
Var &cdr(Var x) {
    return GET(x, Pair).chld[1];
}

String repr(Var v) {
    if (v == nil)
        return "()";
    std::ostringstream sout;
    if (CHECK(v, Number)) {
        sout << GET(v, Number);
    } else if (CHECK(v, String)) {
        sout << GET(v, String);
    } else if (CHECK(v, Boolean)) {
        sout << (GET(v, Boolean) ? "#t" : "#f");
    } else if (CHECK(v, Symbol)) {
        sout << getSymbolString(GET(v, Symbol));
    } else if (CHECK(v, Frame)) {
        sout << "**Frame**";
        auto &mp = GET(v, Frame);
        for (auto e : mp) {
            sout << " (" << getSymbolString(e.first) << ")";
        }
    } else {
        sout << "(";
        while (cdr(v) && CHECK(cdr(v), Pair)) {
            sout << repr(car(v)) << " ";
            v = cdr(v);
        }
        if (cdr(v)) {
            sout << repr(car(v)) << " . " << repr(cdr(v));
        } else {
            sout << repr(car(v));
        }
        sout << ")";
    }
    return sout.str();
}

}; // namespace si