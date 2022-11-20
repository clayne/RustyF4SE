#ifndef RUSTYF4SE_SYMBOL_EXPORT_H
#define RUSTYF4SE_SYMBOL_EXPORT_H

// Cross-platform symbol visibility macro.
#if defined(_MSC_VER)
    #define RUSTYF4SE_SYMBOL_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
    #define RUSTYF4SE_SYMBOL_EXPORT __attribute__((visibility("default")))
#else
    #define RUSTYF4SE_SYMBOL_EXPORT
#endif

#endif // RUSTYF4SE_SYMBOL_EXPORT_H
