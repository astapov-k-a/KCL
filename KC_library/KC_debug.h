#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF

enum {kErrorPrintSleep = 10};

#   ifdef   KCL_NO_KERNEL
#      define DiagnosticPrintServ printf
#      define PrintError printf
#      define Print printf
#      define sprintf __builtin_sprintf
#   else  //KCL_NO_KERNEL
#define Print printk
#      ifdef   __cplusplus
#      endif //__cplusplus

#      ifdef   __cplusplus
extern "C" {
extern void printk(const char *fmt, ...);
#      endif //__cplusplus
#   define DiagnosticPrintServ printk
#   define PrintError(...) { \
      Size i; \
      enum {kLocalRepeats = 2<<8}; \
      printk( __VA_ARGS__); \
      printk(KERN_EMERG "\n"); \
      for(i = 0 - kLocalRepeats; i; ++i) { \
        printk(KERN_CONT ".");\
        KCL_NAMESPACE_PERFIX Shedule(kErrorPrintSleep); \
      } \
      printk(KERN_EMERG "\n"); \
    }
#   define sprintf __builtin_sprintf
#      ifdef   __cplusplus
}
#      endif //__cplusplus
#   endif //KCL_NO_KERNEL

#   ifdef    _DEBUG
#      define DiagnosticPrint DiagnosticPrintServ
#   else  // _DEBUG
#      define DiagnosticPrint(...)
#   endif // _DEBUG

#   define INT3 asm (   "int $3"  );

#   ifndef  KCL_NO_KERNEL
#      define BREAK()
#   else  //KCL_NO_KERNEL
#      ifdef    __GNUC__
#         define BREAK() { __builtin_trap(); }
//#         define BREAK() { raise(SIGTRAP); }
#      else //  __GNUC__
#         define BREAK() { INT3 }
#      endif // __GNUC__
#   endif //KCL_NO_KERNEL


#   ifndef  NO_KERN //для работы в ядре
#      define KCL_ASSERT_SERVICE(condition, message, file, line) {if (!(condition)) {PrintError(KERN_EMERG " * * * ASSERTION_ERROR * * * ---> %s  ( in file %s   at %d ) ", message, file, line); BREAK();} }
#   else  //NO_KERN
#      define KCL_ASSERT_SERVICE(condition, message, file, line) {if (!(condition)) {PrintError(           " * * * ASSERTION_ERROR * * * ---> %s  ( in file %s   at %d ) ", message, file, line); BREAK(); } }
#   endif //NO_KERN
#   define KCL_ASSERT(condition, message) { KCL_ASSERT_SERVICE(condition, message, __FILE__, __LINE__) }

#define CONDITIONAL_BREAK(Condition, Iteration) {static unsigned long __debug_counter = 1; if ( Condition ) { if ( __debug_counter >= (Iteration) ) BREAK(); ++__debug_counter;} }

#define CONDITIONAL_BREAK_N_PRINT(Condition, Iteration, StartPrintIteration, PrintFrequency, Name) \
  {\
    static unsigned long __debug_counter = 1;\
  if ( (__debug_counter >= StartPrintIteration) && (__debug_counter % PrintFrequency) == 0 ) DiagnosticPrint("\nCondition %s, iteration = %lu\n", Name, __debug_counter); \
    if ( Condition ) { \
      if ( __debug_counter >= (Iteration) ) BREAK(); \
      ++__debug_counter; \
    } \
  }

#   ifdef   __cplusplus
#   endif //__cplusplus


#   ifdef   __cplusplus
extern "C" {
#   endif //__cplusplus
extern char const * const kBoolValueNames[2];
#   ifdef   __cplusplus
}
#   endif //__cplusplus


#endif  // KC_LIBRARY_PROTECT_OFF
