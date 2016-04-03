#ifdef   KCL_NO_KERNEL
#   include <stdio.h>
#   include <stdlib.h>
#   include <errno.h>
#   include <unistd.h>
#   include <string.h>
#   include <fcntl.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <sys/types.h>
#   include <signal.h>
#   ifndef      _WIN32
#      include <poll.h>
#      include <pthread.h>
#   endif //not _WIN32
#else  //KCL_NO_KERNEL
#endif //KCL_NO_KERNEL


#   ifndef      _WIN32
#   include <linux/sched.h>

#   endif //not _WIN32

#include "KC_library.h"



enum {kStandardSendFlags = 0,};

///@todo примитив синхронизации для функций памяти в случае отладки памяти
char const * const kBoolValueNames[] = {
  "False",
  "True"
};

Size debug_locker_counter = 0;

#      ifdef    KCL_MEMORY_DEBUG

#         ifdef    KCL_NO_KERNEL

#         endif // KCL_NO_KERNEL

#      endif // KCL_MEMORY_DEBUG


void *KCL_Allocate_C(size_t s) {
#ifndef   KCL_NO_KERNEL
  return kmalloc(s, kAllocateFlags);
#else   //KCL_NO_KERNEL
  return malloc(s);
#endif  //KCL_NO_KERNEL
}

extern void * KCL_Rellocate_C(const void *pointer, Size number_of_bytes) {
#ifndef       KCL_NO_KERNEL
  return krealloc(pointer, number_of_bytes, kAllocateFlags);
#else   //not KCL_NO_KERNEL
  return realloc( (void *)pointer, number_of_bytes);
#endif  //not KCL_NO_KERNEL
}

extern void KCL_FreeOrDie_C(void *p) {
  KCL_ASSERT(p != NULL, "KCL_FreeOrDie_C warning: try to destruct null pointer!!!");
#ifndef   KCL_NO_KERNEL
  kfree(p);
#else   //KCL_NO_KERNEL
  free(p);
#endif  //KCL_NO_KERNEL
}



KCL_EXTERN RandInteger * GetKCLDummy_C() {
  static RandInteger dummy;
  return &dummy;
}

