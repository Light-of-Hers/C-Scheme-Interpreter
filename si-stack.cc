#include "si-stack.h"
#include "si-error.h"
#include <cassert>
#include <vector>
namespace si {

static std::vector<Var> varstk;
static std::vector<void *> contstk;
static const std::string msg = "Stack overflow";
#define MAXDEPTH (1 << 20)

void initStack() {
    varstk.clear(), contstk.clear();
}
void push(Var x) {
    wish(varstk.size() + contstk.size() < MAXDEPTH, msg, SKIP);
    varstk.push_back(x);
}
void push(void *cont) {
    wish(varstk.size() + contstk.size() < MAXDEPTH, msg, SKIP);
    contstk.push_back(cont);
}
void pop(Var &x) {
    x = varstk.back();
    varstk.pop_back();
}
void pop(void *&cont) {
    cont = contstk.back();
    contstk.pop_back();
}

}; // namespace si