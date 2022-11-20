#ifndef RUSTYF4SE_CPP_H
#define RUSTYF4SE_CPP_H

#include <_cxx.h>
#include <memory>
#include <string>
#include "symbol_export.h"

namespace rustyf4se {

    EXPORT_THIS
    void log_info(rust::Str text);
    
    EXPORT_THIS
    void log_warning(rust::Str text);

} // namespace rustyf4se

#endif // RUSTYF4SE_CPP_H
