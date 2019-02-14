#ifndef _si_global_h_
#define _si_global_h_

#include "si-value.h"

namespace si {

void initGlobal();
Var getInteger(Integer i);
Var getNumber(const std::string &n);
Var getBoolean(bool b);

enum class BTSB {
    DEFINE = 0,
    SET,
    LAMBDA,
    BEGIN,
    IF,
    COND,
    OK,
    PROC,
    QUOTE,
    PRIM,
    ELSE,
    AND,
    LET,
    OR,
    OR_TEST,
    WHILE,
    FOR,
    LET_,
    UNASS,
    ARROW,
    COND_TEST,
};

Var getSymbol(Symbol sb);
#define getBTSB(sb) getSymbol((Symbol)BTSB::sb)
Symbol addSymbol(const String &s);
const String getSymbolString(Symbol sb);

}; // namespace si

#endif