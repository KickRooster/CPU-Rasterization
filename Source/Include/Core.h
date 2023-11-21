#pragma once
#include <memory>

namespace core
{
    using int64 = signed long;
    using int32 = signed int;
    using int8 = signed char;

    using uint64 = unsigned long;
    using uint32 = unsigned int;
    using uint8 = unsigned char;

#ifdef _WIN64
    using SIZE_T = unsigned __int64;
    using SSIZE_T = __int64;
#else
    using SIZE_T = unsigned long;
    using SSIZE_T = long;
#endif

    using ANSICHAR = char;
    using WIDECHAR = wchar_t;
}
