#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF
#   ifdef    KCL_NO_KERNEL
#      include <stddef.h>
#   endif // KCL_NO_KERNEL

#   ifdef KCL_SEPARATE_COMPILE_FOR_LINUX_KERNEL
//C-standard like types
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef long long unsigned int uint64_t;
typedef   signed char int8_t;
typedef   signed char int16_t;
typedef   signed char int32_t;
typedef long long   signed int int64_t;
typedef long unsigned int size_t;
typedef signed long ptrdiff_t;
#   endif

#define Debug_KGTypes
//Google-Style types
typedef size_t Size;
typedef char Char;
typedef uint8_t Byte;
typedef uint8_t  UInteger8;
typedef uint16_t UInteger16;
typedef uint32_t UInteger32;
typedef uint64_t UInteger64;
typedef uint8_t   Integer8;
typedef uint16_t  Integer16;
typedef uint32_t  Integer32;
typedef uint64_t  Integer64;
typedef ptrdiff_t PointerDifference;

#   ifndef  __cplusplus
#      ifndef bool
#        ifdef   KCL_NO_KERNEL
typedef uint8_t bool;
#        endif //KCL_NO_KERNEL
#      endif //ifndef bool
#   endif //not __cplusplus

#endif  // KC_LIBRARY_PROTECT_OFF
