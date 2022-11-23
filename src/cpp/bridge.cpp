#include <bridge.h>

#include <iostream>

namespace bridge {

void log_info(rust::Str text) {logger::info(std::string(text));}
void log_warning(rust::Str text) {logger::warn(std::string(text));}

} // namespace bridge