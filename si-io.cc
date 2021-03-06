#include "si-io.h"
#include "si-error.h"
#include "si-global.h"
#include "si-number.h"
#include "si-any.h"
#include <cassert>
#include <sstream>
#include <vector>

namespace si {

static const std::string msg = "Invalid syntax";

struct Token {
    enum class Type {
        LEFT_BRACKET,
        RIGHT_BRACKET,
        DOT,
        QUOTE,
        SYMBOL,
        NUMBER,
        STRING,
        BOOLEAN
    } type;
    any attr;

    Token() = default;
    Token(Type t) : type(t) {
    }
    Token(Number x) : type(Type::NUMBER), attr(x) {
    }
    Token(const String &x) : type(Type::STRING), attr(x) {
    }
    Token(Symbol x) : type(Type::SYMBOL), attr(x) {
    }
    Token(Boolean x) : type(Type::BOOLEAN), attr(x) {
    }
    const String str() const;
};
typedef std::vector<Token> Tokens;

static String split(const String &code) {
    using namespace std;
    string res;
    int len = code.size();
    for (int i = 0; i < len; ++i) {
        char ch = code[i];
        switch (ch) {
            case '"': {
                int j;
                (res += '\n') += code[i];
                for (j = i + 1; j < len && code[j] != '"'; ++j)
                    res += code[j];
                (res += code[j]) += '\n';
                i = j;
                break;
            }
            case '\'':
            case '(':
            case ')': {
                res += string(" ") + ch + string(" ");
                break;
            }
            default: {
                res += ch;
                break;
            }
        }
    }
    return res;
}

static Tokens scan(const String &code) {
    using namespace std;
    Tokens tks;
    // tks.push_back(Token::Type::LEFT_BRACKET);
    // tks.push_back(addSymbol("begin"));

    istringstream input(split(code));
    string line;
    while (getline(input, line)) {
        if (!line.empty()) {
            if (line.front() == '"' && line.back() == '"') {
                tks.push_back(line);
            } else {
                string tk;
                istringstream strin(line);
                while (strin >> tk) {
                    if (tk == "'") {
                        tks.push_back(Token::Type::QUOTE);
                    } else if (tk == ".") {
                        tks.push_back(Token::Type::DOT);
                    } else if (tk == "(") {
                        tks.push_back(Token::Type::LEFT_BRACKET);
                    } else if (tk == ")") {
                        tks.push_back(Token::Type::RIGHT_BRACKET);
                    } else if (tk == "#t" || tk == "#f") {
                        tks.push_back((bool)(tk == "#t"));
                    } else {
                        try {
                            tks.push_back(ston(tk));
                        } catch (std::invalid_argument) {
                            tks.push_back(addSymbol(tk));
                        }
                    }
                }
            }
        }
    }
    // tks.push_back(Token::Type::RIGHT_BRACKET);
    return tks;
}

const String Token::str() const {
    using namespace std;
    ostringstream sout;
    if (type == Type::LEFT_BRACKET) {
        sout << "<(>";
    } else if (type == Type::RIGHT_BRACKET) {
        sout << "<)>";
    } else if (type == Type::QUOTE) {
        sout << "<'>";
    } else if (type == Type::DOT) {
        sout << "<.>";
    } else if (type == Type::NUMBER) {
        sout << "<number, " << any_cast<Number>(attr) << ">";
    } else if (type == Type::STRING) {
        sout << "<string, " << any_cast<String>(attr) << ">";
    } else if (type == Type::SYMBOL) {
        sout << "<symbol, " << any_cast<Symbol>(attr) << ">";
    } else if (type == Type::BOOLEAN) {
        sout << "<boolean, " << any_cast<Boolean>(attr) << ">";
    } else {
        return "";
    }
    return sout.str();
}

static int step;
Var parse_aux(const Tokens &tks) {
    Token tk = tks[step++];
    if (tk.type == Token::Type::NUMBER) {
        return make(any_cast<Number>(tk.attr));
    } else if (tk.type == Token::Type::STRING) {
        return make(any_cast<String>(tk.attr));
    } else if (tk.type == Token::Type::BOOLEAN) {
        return getBoolean(any_cast<Boolean>(tk.attr));
    } else if (tk.type == Token::Type::SYMBOL) {
        return getSymbol(any_cast<Symbol>(tk.attr));
    } else if (tk.type == Token::Type::LEFT_BRACKET) {
        Var head = cons(nil, nil);
        Var cur = head;
        while (tks[step].type != Token::Type::RIGHT_BRACKET) {
            if (tks[step].type == Token::Type::DOT) {
                step++;
                cdr(cur) = parse_aux(tks);
                wish(tks[step].type == Token::Type::RIGHT_BRACKET, msg, SKIP);
                break;
            }
            cdr(cur) = cons(parse_aux(tks), nil);
            cur = cdr(cur);
        }
        step++;
        return cdr(head);
    } else if (tk.type == Token::Type::QUOTE) {
        return cons(getBTSB(QUOTE), cons(parse_aux(tks), nil));
    } else {
        return nil;
    }
}

static Var parse(const String &code) {
    step = 0;
    return parse_aux(scan(code));
}

std::istream &operator>>(std::istream &is, Var &x) {
    String code;
    int lb = 0, rb = 0;
    bool quoted = false;
    bool ok = false;
    char ch;
    while (is.get(ch)) {
        code.push_back(ch);
        if (ch == '"')
            quoted = !quoted;
        if (!quoted) {
            if (ch == '(') {
                lb++;
            } else if (ch == ')') {
                rb++;
            } else if (ch == '\n') {
                if (lb == rb) {
                    ok = true;
                    break;
                }
            }
        }
    }
    if (ok)
        x = parse(code);
    return is;
}

std::ostream &operator<<(std::ostream &os, Var x) {
    os << repr(x);
    return os;
}

}; // namespace si