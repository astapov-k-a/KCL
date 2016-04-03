#ifndef KC_LIBRARY_DECLARATIONS_H
#define KC_LIBRARY_DECLARATIONS_H

#define KCL_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&);

#   ifdef    KCL_MEMORY_DEBUG_ACCESS
#      ifdef   __cplusplus

#         define KCL_POINTER(ObjectType) KCL_NAMESPACE_PERFIX SafePointer<ObjectType>
#         define CONST_KCL_POINTER(ObjectType) KCL_NAMESPACE_PERFIX SafeConstPointer<ObjectType>
#         define KCL_WEAK_POINTER(ObjectType) KCL_NAMESPACE_PERFIX SafePointer<ObjectType>
#         define CONST_KCL_WEAK_POINTER(ObjectType) KCL_NAMESPACE_PERFIX SafeConstPointer<ObjectType>

#         define KCL_ARRAY_POINTER(Type) KCL_NAMESPACE_PERFIX ArrayPointer<Type>
#         endif //__cplusplus
#   else  // KCL_MEMORY_DEBUG_ACCESS
#      define KCL_POINTER(ObjectType) ObjectType *
#      define CONST_KCL_POINTER(ObjectType) const ObjectType *
#      define KCL_WEAK_POINTER(ObjectType) ObjectType *
#      define CONST_KCL_WEAK_POINTER(ObjectType) const ObjectType *
#      define KCL_ARRAY_POINTER(Type) KCL_NAMESPACE::ArrayPointer<Type>
#   endif // KCL_MEMORY_DEBUG_ACCESS


#   ifdef    KCL_MEMORY_DEBUG
#      define KCL_CREATE_OBJECT(ObjectType) KCL_NAMESPACE_PERFIX CreateObjectDebug<ObjectType>(__FILE__, __LINE__)
#      define KCL_DELETE_OBJECT(pointer)    KCL_NAMESPACE_PERFIX DeleteObjectDebug(pointer, __FILE__, __LINE__)
#   else  // KCL_MEMORY_DEBUG
#      define KCL_CREATE_OBJECT(ObjectType) KCL_NAMESPACE_PERFIX CreateObject<ObjectType>()
#      define KCL_DELETE_OBJECT(pointer)    KCL_NAMESPACE_PERFIX DeleteObject(pointer)
#   endif // KCL_MEMORY_DEBUG

#   ifndef KERN_INFO
#      ifdef   KCL_NO_KERNEL
#         define	KERN_EMERG   "\n"
#         define	KERN_ALERT   "\n"
#         define	KERN_CRIT    "\n"
#         define	KERN_ERR     "\n"
#         define	KERN_WARNING "\n"
#         define	KERN_NOTICE  "\n"
#         define	KERN_INFO    "\n"
#         define	KERN_DEBUG   "\n"
#         define  KERN_DEFAULT "\n"
#         define	KERN_CONT
#      else  //KCL_NO_KERNEL
#         define	KERN_EMERG	 "<0>"	// system is unusable
#         define	KERN_ALERT	 "<1>"	// action must be taken immediately
#         define	KERN_CRIT	   "<2>"	// critical conditions
#         define	KERN_ERR	   "<3>"	// error conditions
#         define	KERN_WARNING "<4>"	// warning conditions
#         define	KERN_NOTICE	 "<5>"	// normal but significant condition
#         define	KERN_INFO	   "<6>"	// informational
#         define	KERN_DEBUG	 "<7>"	// debug-level messages
#         define  KERN_DEFAULT "<d>"   // Use the default kernel loglevel
#         define	KERN_CONT	"<c>"
#      endif //KCL_NO_KERNEL
#   endif //KERN_INFO

KCL_INLINE void MemoryZeroizing(const void * memory_block, Size number_bytes) {
  memset( (void*) memory_block, 0, number_bytes);
}

KCL_INLINE void MemoryCopy(const void * source, Size number_bytes, void * destination) {
  memcpy(destination, source, number_bytes);
}

KCL_INLINE Size GetUnsafeStringLength(const Char * string) {
  return strlen(string);
}

enum {kStringTerminator = 0};
KCL_INLINE Size GetSafeStringLength(const Char * string, bool * out_is_error) {
  bool is_error;
  Size re = (Size)(-1);
  Size limit = re - (Size)string;
  const Char * memchr_retvalue = (const Char *)memchr(string, kStringTerminator ,limit);
  is_error = memchr_retvalue < string;
  if (!is_error) {
    re = memchr_retvalue - string;
  }
  if (out_is_error) out_is_error[0] = is_error;
  return re;
}

KCL_INLINE Size GetStringLength(const Char * string, bool * out_is_error) {
  return GetSafeStringLength(string, out_is_error);
}

KCL_EXTERN void * Allocate(Size number_of_bytes);
KCL_EXTERN void   DeallocateOrDie(const void * pointer);
KCL_EXTERN void   SafeDeallocate(const void * pointer);
KCL_EXTERN void * Reallocate(const void *pointer, Size number_of_bytes);
KCL_EXTERN const Char * GetErrorTextForSafePointers(Size error_code, Size operator_code);
KCL_EXTERN void SaveAndDisableLocalIrq(unsigned long * out_for_save);
KCL_EXTERN void RestoreLocalIrq(unsigned long const * saved);
KCL_EXTERN void SaveAndDisableLocalIrqBH(void);
KCL_EXTERN void RestoreLocalIrqBH(void);


KCL_EXTERN void CPP_EnvironmentInstall(void);
KCL_EXTERN void   InitializeMemoryDebugger(void);
KCL_EXTERN void UninitializeMemoryDebugger(void);
#   ifdef    KCL_MEMORY_DEBUG
#      ifdef   __cplusplus
namespace KCL_NAMESPACE { //namespace kcl started
class MemoryMap;
template <typename Tn> class ArrayPointer;
extern MemoryMap g_memory_map;
template <typename Tn> ArrayPointer<Tn> CreateArrayDebug(Size number_elements, const Char * file, int line);
template <typename Tn> void DeleteArrayDebug(ArrayPointer<Tn> what_delete, const Char * file, int line);
} //namespace kcl finished
#      endif //__cplusplus
KCL_EXTERN bool   FindLeaks(void);
KCL_EXTERN bool   FindLeaksAndAlert(void);
KCL_EXTERN void   GetSnapshotMemoryDebugger(void * snapshot_pointer);
KCL_EXTERN bool   IsEqualMemoryDebugger(const void * snapshot_pointer, bool need_alert);
KCL_EXTERN void UninitializeMemoryDebugger(void);
KCL_EXTERN void   InitializeDebugMemoryLock(void);
KCL_EXTERN void UninitializeDebugMemoryLock(void);
KCL_EXTERN void ReadLockDebugMemoryLocker(void);
KCL_EXTERN void ReadUnlockDebugMemoryLocker(void);
KCL_EXTERN void WriteLockDebugMemoryLocker(void);
KCL_EXTERN void WriteUnlockDebugMemoryLocker(void);
KCL_EXTERN void UpgradeDebugMemoryLocker(void);
#   ifdef   __cplusplus
#   endif //__cplusplus
#   endif // KCL_MEMORY_DEBUG


#   ifdef   __cplusplus
namespace KCL_NAMESPACE { //namespace kcl started

template <typename Tn> class BasePointer;
template <typename Tn> class BaseConstPointer;
template <typename Tn> class SafePointer;
template <typename Tn> class SafeConstPointer;

template <typename Tn> class Vector;
template <typename Tn> class Allocator;
template <typename IntegerTn> class BaseFlags;
template <Size DimensionX, Size DimensionY> class BitField;


template <typename Tn> class PointerLimits;

template <typename Tn> const char * GetTypeName();
template <typename Tn> KCL_POINTER(Tn) CreateObject();
template <typename Tn> inline Size SizeOf() {return sizeof(Tn);}
template <typename Tn> inline Size SizeOf(Tn&) {return SizeOf<Tn>();}
template <typename Tn> KCL_INLINE Tn * CreateObjectInternal();
template <typename Tn> void DeleteObject(KCL_POINTER(Tn) what_delete);
template <typename Tn> KCL_POINTER(Tn) CreateObjectDebug(const Char * file, int line);
template <typename Tn> void DeleteObjectDebug(const Char * file, int line, KCL_POINTER(Tn) what_delete);

template <typename Tn> void Construct(Tn * address);
template <typename Tn> void Destruct(Tn & object);
template <typename Tn> void Destruct( KCL_POINTER(Tn) pointer);
template <typename Tn> KCL_POINTER(Tn) CreatePointer(      Tn * address);
template <typename Tn> KCL_POINTER(Tn) CreatePointer(const Tn * address);
template <typename Tn> Tn * GetAddressFromPointer( KCL_POINTER(Tn) address);

template <typename Tn> CONST_KCL_POINTER(Tn) ConstCastPointer( KCL_POINTER(Tn) address);
template <typename Tn> KCL_POINTER(Tn) UnconstCastPointer(CONST_KCL_POINTER(Tn) address);
template <typename Tn> CONST_KCL_WEAK_POINTER(Tn) ConstCastWeakPointer (KCL_WEAK_POINTER(Tn) address);
template <typename Tn> KCL_WEAK_POINTER(Tn) UnconstCastWeakPointer (CONST_KCL_WEAK_POINTER(Tn) address);
template <typename Tn> KCL_WEAK_POINTER(Tn) StrongToWeak ( KCL_POINTER(Tn) pointer);
template <typename Tn> KCL_WEAK_POINTER(Tn) AddressToWeak ( Tn * pointer);

KCL_EXTERN void Shedule(unsigned long timeout);

#      define KCL_GET_INFINUM_SIZE(total_size, item_size) ( ( (total_size) / (item_size) ) + ( (total_size) % (item_size) != 0) )
KCL_HEADER_DEFINITION Size GetInfinumSize(Size total_size, Size item_size) {return KCL_GET_INFINUM_SIZE (total_size, item_size);}
KCL_HEADER_DEFINITION  Size Align(Size number, Size align_base) { return GetInfinumSize(number, align_base) * align_base; }
} //namespace kcl finished
#   else  //__cplusplus
KCL_EXTERN void Shedule(unsigned long timeout);
#   endif //__cplusplus

#   ifndef NULL
#      define NULL    ((void *)0UL)
#   endif


#endif // KC_LIBRARY_DECLARATIONS_H
