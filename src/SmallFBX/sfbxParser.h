#pragma once

namespace sfbx {

template <typename T> inline void append(std::string &dst, T v) {
    std::ostringstream buf;
    buf << v;
    dst += buf.str();
}
template <class String, class Container, class ToString>
inline void join(String &dst, const Container &cont, typename String::const_pointer sep, const ToString &to_s) {
    bool first = true;
    for (auto &v : cont) {
        if (!first) dst += sep;
        to_s(dst, v);
        first = false;
    }
}
template <class String, class Container> inline void join(String &dst, const Container &cont, const char *sep) {
    join(dst, cont, sep, [](String &d, typename Container::const_reference v) { append(d, v); });
}

} // namespace sfbx
