#ifndef _si_any_h_
#define _si_any_h_

#include <algorithm>
#include <type_traits>
#include <typeinfo>

namespace si {

class any {
    struct holder {
        virtual holder *clone() const = 0;
        virtual const std::type_info &type() const noexcept = 0;
        virtual ~holder() = default;
    };
    template <typename T>
    struct holder_impl : public holder {
        T val;
        holder_impl(const T &v) : val(v) {}
        holder_impl(const holder_impl &rhs) : val(rhs.val) {}
        holder *clone() const override { return new holder_impl(val); }
        const std::type_info &type() const noexcept override {
            return typeid(T);
        }
    };

    holder *hdr;
    holder *clone() const { return hdr ? hdr->clone() : nullptr; }

  public:
    any() : hdr(nullptr) {}
    template <typename T>
    any(const T &v) : hdr(new holder_impl<T>(v)) {}
    any(any &&rhs) noexcept : hdr(rhs.hdr) { rhs.hdr = nullptr; }
    any(const any &rhs) : hdr(rhs.clone()) {}

    ~any() { reset(); }

    any &operator=(const any &rhs) {
        if (this != &rhs)
            reset(), hdr = rhs.clone();
        return *this;
    }
    any &operator=(any &&rhs) noexcept {
        if (this != &rhs)
            hdr = rhs.hdr, rhs.hdr = nullptr;
        return *this;
    }
    template <typename T>
    any &operator=(const T &v) {
        reset(), hdr = new holder_impl<T>(v);
        return *this;
    }

    inline bool has_value() const noexcept { return hdr != nullptr; }
    const std::type_info &type() const noexcept { return hdr->type(); }

    void swap(any &rhs) noexcept { std::swap(hdr, rhs.hdr); }
    inline void reset() noexcept {
        if (hdr)
            delete hdr, hdr = nullptr;
    }

    template <typename T>
    friend T any_cast(const any &);
};

template <typename T>
T any_cast(const any &a) {
    return static_cast<
               any::holder_impl<typename std::remove_reference<T>::type> *>(
               a.hdr)
        ->val;
}

template <typename T>
any make_any(const T &v) {
    return any(v);
}

}; // namespace si

#endif