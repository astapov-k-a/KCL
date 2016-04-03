#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF

#   ifdef   __cplusplus
void *operator new(size_t size, void *p) throw();
#   endif //__cplusplus

#endif  // KC_LIBRARY_PROTECT_OFF
