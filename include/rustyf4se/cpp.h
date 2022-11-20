#ifndef RUSTYF4SE_CPP_H
#define RUSTYF4SE_CPP_H

#include <_cxx.h>
#include <memory>
#include <string>
#include "symbol_export.h"

namespace rustyf4se {

    class ThingC {
    public:
        RUSTYF4SE_SYMBOL_EXPORT
        ThingC(std::string appname);

        RUSTYF4SE_SYMBOL_EXPORT
        ~ThingC();

        std::string appname;
    };

    struct SharedThing;

    RUSTYF4SE_SYMBOL_EXPORT
    std::unique_ptr <ThingC> make_demo(rust::Str appname);

    RUSTYF4SE_SYMBOL_EXPORT
    const std::string &get_name(const ThingC &thing);

    RUSTYF4SE_SYMBOL_EXPORT
    void do_thing(SharedThing state);

} // namespace rustyf4se

#endif // RUSTYF4SE_CPP_H
