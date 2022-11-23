#ifndef BRIDGE_CPP_H
#define BRIDGE_CPP_H

#define EXPORT_THIS __declspec(dllexport)

#include <_cxx.h>
#include <_cxxbridge.h>

#include <memory>
#include <string>

namespace bridge {

    EXPORT_THIS
    void log_info(rust::Str text);
    
    EXPORT_THIS
    void log_warning(rust::Str text);

} // namespace bridge

#endif // BRIDGE_CPP_H
