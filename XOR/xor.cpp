// XOR.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include <xor.h>

// TODO: This is an example of a library function
namespace std {

    void w_printf(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }

    void w_printf_s(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf_s(fmt, args);
        va_end(args);
    }

    void w_sprintf(char* buf, size_t buf_size, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, buf_size, fmt, args);
        va_end(args);
    }

    int w_sprintf_ret(char* buf, size_t buf_size, const char* fmt, ...) {
        int ret;
        va_list args;
        va_start(args, fmt);
        ret = vsnprintf(buf, buf_size, fmt, args);
        va_end(args);
        return ret;
    }

    void w_sprintf_s(char* buf, size_t buf_size, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vsprintf_s(buf, buf_size, fmt, args);
        va_end(args);
    }

    int w_sprintf_s_ret(char* buf, size_t buf_size, const char* fmt, ...) {
        int ret;
        va_list args;
        va_start(args, fmt);
        ret = vsprintf_s(buf, buf_size, fmt, args);
        va_end(args);
        return ret;
    }

} // namespace std