#include "si-global.h"
#include <map>
#include <vector>

namespace si {

typedef std::weak_ptr<Value> VarWatcher;

static std::map<Integer, VarWatcher> integers;
static Var booleans[2];
static std::vector<Var> symbols;
static std::map<String, Symbol> str2sb;
static std::vector<String> sb2str;

static std::vector<String> gbsb{
    "define",
    "set!",
    "lambda",
    "begin",
    "if",
    "cond",
    "ok",
    "**procedure**",
    "quote",
    "**primitive-procedure**",
    "else",
    "and",
    "let",
    "or",
    "**or-test**",
    "while",
    "for",
    "let*",
    "**unassigned**",
    "=>",
    "**cond-test**",
};

void initGlobal() {
    booleans[0] = make(false), booleans[1] = make(true);
    symbols.clear(), str2sb.clear(), sb2str.clear(), integers.clear();
    for (const auto &s : gbsb)
        (void)addSymbol(s);
}

Symbol addSymbol(const String &s) {
    auto it = str2sb.find(s);
    if (it == str2sb.end()) {
        Symbol sb = symbols.size();
        str2sb[s] = sb;
        sb2str.push_back(s);
        symbols.push_back(make((Symbol)sb));
        return sb;
    } else {
        return it->second;
    }
}
const String getSymbolString(Symbol sb) {
    return sb2str[sb];
}
Var getSymbol(Symbol sb) {
    return symbols[sb];
}

Var getBoolean(bool b) {
    return booleans[(int)b];
}

static constexpr size_t ints_cap = 200;
static constexpr size_t check_time = 100;
static size_t cur_time = 0;

static void sweap() {
    if (integers.size() < ints_cap)
        return;
    auto next = integers.begin();
    auto cur = next;
    next++;
    while (next != integers.end()) {
        if (cur->second.expired())
            integers.erase(cur);
        cur = next;
        next++;
    }
    if (cur->second.expired())
        integers.erase(cur);
}

Var getInteger(Integer i) {
    auto it = integers.find(i);
    Var res;
    if (it == integers.end()) {
        res = make(i);
        integers.insert({i, res});
    } else if (it->second.expired()) {
        res = make(i);
        it->second = res;
    } else {
        res = it->second.lock();
    }
    if (++cur_time > check_time)
        sweap(), cur_time = 0;
    return res;
}

Var getNumber(const String &n) {
    return make(std::stod(n));
}

}; // namespace si