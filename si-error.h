#ifndef _si_error_h_
#define _si_error_h_

#include <string>

namespace si {

struct Error {
    enum class Handle { SKIP, REBOOT, ABORT } handle;
    std::string msg;
};

#define wish(pred, msg, handle)                                                \
    ((pred) ? (void)0 : throw Error({Error::Handle::handle, msg}))

#define wishtype(x, t)                                                         \
    wish(CHECK(x, t),                                                          \
         std::string("Incorrect type of ") + repr(x) + ", expect " +           \
             typeid(t).name() + ", given " + (x ? x->type.name() : "null"),    \
         SKIP)

#define unbound(x)                                                             \
    wish(false, std::string("Unbound variable: " + repr(x)), SKIP)

}; // namespace si

#endif