#ifndef   KC_LIBRARY_H
#define   KC_LIBRARY_H

//#define KCL_ALIEN_OFF

#   ifndef       __PRETTY_FUNCTION__
#      ifdef    __BORLANDC__
#         define __PRETTY_FUNCTION__ __func__
#      endif // __BORLANDC__

#      ifdef    _MSC_VER
#         define __PRETTY_FUNCTION__ __FUNCSIG__
#      endif // _MSC_VER

#      ifndef       __PRETTY_FUNCTION__
#         define __PRETTY_FUNCTION__ "Function name unsupported for this compiler"
#      endif // not __PRETTY_FUNCTION__
#   endif // not __PRETTY_FUNCTION__


#   ifdef    __GNUC__
#      define KCL_FORCE_INLINE_SERV inline __attribute__((always_inline))
#   else  // __GNUC__
#      ifdef    _MSC_VER
#         define KCL_FORCE_INLINE_SERV __forceinline
#      endif // _MSC_VER
#      ifndef KCL_FORCE_INLINE_SERV
#         define KCL_FORCE_INLINE_SERV inline
#      endif
#   endif // __GNUC__


#   ifdef    NO_FORCE_INLINE
#      define KCL_FORCE_INLINE
#   else  // NO_FORCE_INLINE
#      define KCL_FORCE_INLINE KCL_FORCE_INLINE_SERV /// @debug нужно разремить эту строку и убрать следующую
//#      define KCL_FINLINE /// @debug нужно убрать эту строку и разремить предыдущую
#   endif // NO_FORCE_INLINE

#   define KCL_FINLINE KCL_FORCE_INLINE // сокращение от KCL_FORCE_INLINE

#define KCL_HEADER_DEFINITION inline
#define KCL_INLINE_HEADER_DEFINITION KCL_FORCE_INLINE_SERV

#   define KCL_USED(x) {  (void )(x);  }

#   ifndef    KCL_NO_KERNEL
#      ifdef    __cplusplus
#         define KCL_CPP_LINUX_KERNEL // Принак того, что библиотека собирается под ядром linux и ПРИ ЭТОМ сейчас компилируется c++ код
#      endif // __cplusplus
#   endif  // KCL_NO_KERNEL

#   ifndef     _WIN32
#      ifndef   KCL_SEPARATE_COMPILE_FOR_LINUX_KERNEL
#         include <linux/types.h>
#         ifndef   KCL_NO_KERNEL
#            include <linux/module.h>
#            include <linux/kernel.h>
#         endif // KCL_NO_KERNEL
#      endif  //KCL_SEPARATE_COMPILE_FOR_LINUX_KERNEL
#   else  //_WIN32
#      include <string.h>
#   endif //_WIN32


#   ifdef    KCL_MEMORY_DEBUG
#      ifndef   _DEBUG
#         error Key 'KCL_MEMORY_DEBUG' without '_DEBUG' key!
#      endif // _DEBUG
#   endif // KCL_MEMORY_DEBUG


#   ifdef    KCL_MEMORY_DEBUG_ACCESS
#      ifndef   _DEBUG
#         error Key 'KCL_MEMORY_DEBUG_ACCESS' without '_DEBUG' key!
#      endif // not _DEBUG
#      ifndef    KCL_MEMORY_DEBUG
#         error Key 'KCL_MEMORY_DEBUG_ACCESS' without 'KCL_MEMORY_DEBUG' key!
#      endif //  not KCL_MEMORY_DEBUG
#   endif // KCL_MEMORY_DEBUG_ACCESS


#   ifdef   __cplusplus
extern "C" {
#   endif //__cplusplus



#   ifdef  KCL_NO_KERNEL
#      include <stdint.h>
#   else  //KCL_NO_KERNEL
#   endif //KCL_NO_KERNEL


#   ifdef   __cplusplus
}
#   endif //__cplusplus

#   ifndef _WIN32
#      include <linux/string.h>
#   endif
#   include <stdarg.h>
#   include <stdint.h>
#   include <stddef.h>
//#define KCL_MEMORY_DEBUG


#   ifdef   __cplusplus
#      define KCL_EXTERN extern "C"
#      ifndef    KCL_USER_NAMESPACE /// @warning ключ KCL_USER_NAMESPACE позволяет задать библиотеку KC в другом namespace
#         define KCL_NAMESPACE kcl
#      else   // KCL_USER_NAMESPACE
#         define KCL_NAMESPACE KCL_USER_NAMESPACE
#      endif
#      define KCL_NAMESPACE_PERFIX KCL_NAMESPACE::
#   else  //__cplusplus
#      define KCL_EXTERN extern
#      define KCL_NAMESPACE
#      define KCL_NAMESPACE_PERFIX
#   endif //__cplusplus


#   ifdef   __cplusplus
namespace KCL_NAMESPACE {}
template <typename Tn> void Swap(Tn & a, Tn & b) {Tn temp; temp = b; b = a; a = temp;}
#   endif //__cplusplus

#   define KCL_ALLOCATE(number_of_bytes) Allocate(number_of_bytes)
#   define KCL_DEALLOCATE(pointer)       SafeDeallocate(pointer)

#   define KCL_INLINE static inline

#   define Max(a,b)  (  ( (a) > (b) ) ? (a) : (b)  )
#   define Min(a,b)  (  ( (a) < (b) ) ? (a) : (b)  )



#   ifdef   KCL_NO_KERNEL
#      include "stdio.h"
#      include "stdlib.h"
//#      include "conio.h"
#   else  //KCL_NO_KERNEL
#   endif //KCL_NO_KERNEL


#   define KC_LIBRARY_PROTECT_OFF

#   include "KC_types.h"

#   ifdef   KCL_SEPARATE_COMPILE_FOR_LINUX_KERNEL
KCL_INLINE void * memcpy(void * dest, const void * src, size_t n) {return __builtin_memcpy( (dest), (src), (n) );}
KCL_INLINE void * memset( void * ptr, int value, size_t num ) {return __builtin_memset( (ptr), (value), (num) );}
KCL_INLINE Size strlen(const Char * ptr ) {return __builtin_strlen( (ptr) );}
KCL_INLINE void * memchr ( const Char * ptr, int value, size_t num ) {return __builtin_memchr( (ptr), (value), (num) );}
//KCL_INLINE void va_start(__va_list_tag * tag, va_list args) {__builtin_va_start( tag, args );}
#      define va_start(v,l)	__builtin_va_start(v,l)
#   endif //KCL_SEPARATE_COMPILE_FOR_LINUX_KERNEL

#   include "KC_library_declarations.h"

#   ifdef   KCL_NO_NEW_HEADER
#      include "new"
#   else   //KCL_NO_NEW_HEADER
#      include "KC_new.h"
#   endif  //KCL_NO_NEW_HEADER

#   include "KC_debug.h"


#   ifdef   __cplusplus
namespace KCL_NAMESPACE { //namespace kcl started

#   endif //__cplusplus

#   include "KC_random.h"
#   include "KC_pointers.h"
#   include "KC_memory.h"
#   include "KC_list.h"
#   include "KC_vector.h"
#   include "KC_pod_array.h"
#   include "KC_bitfield.h"
#   include "KC_string.h"

#   undef KC_LIBRARY_PROTECT_OFF

#   ifdef   __cplusplus
} //namespace kcl finished
#   endif //__cplusplus

#endif  //KC_LIBRARY_H
