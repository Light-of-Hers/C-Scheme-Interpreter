#ifndef _si_value_h_
#define _si_value_h_

#include "si-error.h"
#include <any>
#include <map>
#include <memory>
#include <string>

namespace si {

struct Value;

typedef std::shared_ptr<Value> Var;
typedef std::string String;
typedef double Real;
typedef long long Integer;
typedef unsigned int Symbol;
typedef bool Boolean;
typedef std::map<Symbol, Var> Frame;

struct Pair {
    Var chld[2];
};

struct Value {
    const std::type_info &type;
    std::any data;
    template <class T> Value(const T &t) : type(typeid(T)), data(t) {
    }
};
template <class T> Var make(const T &t) {
    return std::make_shared<Value>(t);
}
Var cons(Var a, Var b);
Var &car(Var x);
Var &cdr(Var x);
String repr(Var x);
#define nil nullptr
#define guard cons(nil, nil)
#define CHECK(x, t) ((x) && (x)->type == typeid(t))
template <class T> T &_get_value_data_(Var x) {
    wishtype(x, T);
    return std::any_cast<T &>(x->data);
}
#define GET(x, t) _get_value_data_<t>(x)

}; // namespace si

#endif