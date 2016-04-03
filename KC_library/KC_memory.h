#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF



//#   define MAKE_USED(var) {(void)(var);}

#   ifndef      __cplusplus
#      ifndef       GFP_ATOMIC
#         define GFP_ATOMIC 0
#      endif // not GFP_ATOMIC
enum {kAllocateFlags = GFP_ATOMIC};
#   endif //NOT __cplusplus

KCL_EXTERN void *KCL_Allocate_C(size_t size);
KCL_EXTERN void *KCL_Rellocate_C(const void *pointer, Size new_size_in_bytes);
KCL_EXTERN void KCL_FreeOrDie_C(void *p);
KCL_EXTERN void SafeFree_C(void *p);
KCL_EXTERN void * Allocate(Size number_of_bytes);
KCL_EXTERN void  DeallocateOrDie(const void * pointer);
KCL_EXTERN void * Reallocate(const void *pointer, Size number_of_bytes);




#   ifdef   __cplusplus

template <typename Tn> void Construct(Tn * address) {
  new (address) Tn;
}

template <typename Tn> void Destruct(Tn & object) {
  //DiagnosticPrint(KERN_INFO "Destruct object");
  object.~Tn();
}

template <typename Tn> void Destruct( KCL_POINTER(Tn) pointer) {
  pointer->~Tn();
}

#   ifdef    KCL_MEMORY_DEBUG_ACCESS
template <typename Tn> KCL_POINTER(Tn) CreatePointer(      Tn * address, Tn* low_limit, Tn * high_postend) {
  void * last_correct_byte = reinterpret_cast<Byte*>(high_postend) - 1;
  return KCL_POINTER(Tn) (address, low_limit, last_correct_byte);
}

template <typename Tn> KCL_POINTER(Tn) CreatePointer(const Tn * address, const Tn* low_limit, const Tn * high_postend) {
  const void * last_correct_byte = reinterpret_cast<const Byte*>(high_postend) - 1;
  return KCL_POINTER(Tn) ( const_cast<Tn *>(address), low_limit, last_correct_byte );
}

template <typename Tn> KCL_POINTER(Tn) CreatePointer(      Tn * address, Tn* low_limit) {
  void * last_correct_byte = reinterpret_cast<Byte*>(low_limit) + SizeOf<Tn>() - 1;
  return KCL_POINTER(Tn) (address, low_limit, last_correct_byte);
}

template <typename Tn> KCL_POINTER(Tn) CreatePointer(const Tn * address, const Tn* low_limit) {
  const void * last_correct_byte = reinterpret_cast<const Byte*>(low_limit) + SizeOf<Tn>() - 1;
  return KCL_POINTER(Tn) ( const_cast<Tn *>(address), low_limit, last_correct_byte );
}
#   else  // KCL_MEMORY_DEBUG_ACCESS

template <typename Tn> KCL_POINTER(Tn) CreatePointer(      Tn * address) {
  return address;
}

template <typename Tn> KCL_POINTER(Tn) CreatePointer(const Tn * address) {
  return const_cast<Tn *>(address);
}
#   endif // KCL_MEMORY_DEBUG_ACCESS


template <typename Tn> Tn * GetAddressFromPointer
    ( KCL_POINTER(Tn) address) {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return address.GetData();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return address;
#   endif // KCL_MEMORY_DEBUG_ACCESS

}

template <typename Tn> const Tn * GetAddressFromPointer
    ( CONST_KCL_POINTER(Tn) address) {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return address.Data();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return address;
#   endif // KCL_MEMORY_DEBUG_ACCESS

}

template <typename Tn> CONST_KCL_POINTER(Tn) ConstCastPointer
    ( KCL_POINTER(Tn) address) {
//    ( Tn* address) {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return address.ConstCast();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return const_cast< CONST_KCL_POINTER(Tn) >(address);
#   endif // KCL_MEMORY_DEBUG_ACCESS
}

template <typename Tn> KCL_POINTER(Tn) UnconstCastPointer
    (CONST_KCL_POINTER(Tn) address) {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return address.Unconst();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return const_cast< KCL_POINTER(Tn) >(address);
#   endif // KCL_MEMORY_DEBUG_ACCESS
}

template <typename Tn> CONST_KCL_WEAK_POINTER(Tn) ConstCastWeakPointer
    (KCL_WEAK_POINTER(Tn) address) {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return address.ConstCast();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return const_cast< CONST_KCL_WEAK_POINTER(Tn) >(address);
#   endif // KCL_MEMORY_DEBUG_ACCESS
}

template <typename Tn> KCL_WEAK_POINTER(Tn) UnconstCastWeakPointer
    (CONST_KCL_WEAK_POINTER(Tn) address) {
  return const_cast< KCL_WEAK_POINTER(Tn) >(address);
}

template <typename Tn> KCL_WEAK_POINTER(Tn) StrongToWeak ( KCL_POINTER(Tn) pointer) {
  return pointer;
}

template <typename Tn> KCL_WEAK_POINTER(Tn) AddressToWeak ( Tn * pointer) {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  char const * high_limit = (char const *) pointer;
  high_limit += sizeof(Tn) - 1;
  return KCL_WEAK_POINTER(Tn)(pointer, pointer, high_limit);
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return pointer;
#   endif // KCL_MEMORY_DEBUG_ACCESS
}

template <typename Tn> KCL_WEAK_POINTER(Tn) AddressToWeak ( Tn * pointer, Size n_valid_bytes) {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  char const * high_limit = (char const *) pointer;
  high_limit += n_valid_bytes;
  return KCL_WEAK_POINTER(Tn)(pointer, pointer, high_limit);
#   else  // KCL_MEMORY_DEBUG_ACCESS
  KCL_USED(n_valid_bytes);
  return pointer;
#   endif // KCL_MEMORY_DEBUG_ACCESS
}

template <typename Tn> KCL_INLINE
KCL_WEAK_POINTER(Tn) GetNullWeak() {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return KCL_WEAK_POINTER(Tn)::GetNullObject();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return static_cast<KCL_WEAK_POINTER(Tn)>(NULL);
#   endif // KCL_MEMORY_DEBUG_ACCESS
 }

template <typename Tn> KCL_INLINE
CONST_KCL_WEAK_POINTER(Tn) GetNullConstWeak() {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return CONST_KCL_WEAK_POINTER(Tn)::GetNullObject();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return static_cast<CONST_KCL_WEAK_POINTER(Tn)>(NULL);
#   endif // KCL_MEMORY_DEBUG_ACCESS
 }

template <typename Tn> KCL_INLINE
KCL_POINTER(Tn) GetNullStrong() {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return KCL_POINTER(Tn)::GetNullObject();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return static_cast<KCL_POINTER(Tn)>(NULL);
#   endif // KCL_MEMORY_DEBUG_ACCESS
 }

template <typename Tn> KCL_INLINE
CONST_KCL_POINTER(Tn) GetNullConstStrong() {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return CONST_KCL_POINTER(Tn)::GetNullObject();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return static_cast<CONST_KCL_POINTER(Tn)>(NULL);
#   endif // KCL_MEMORY_DEBUG_ACCESS
 }

template <typename Tn> KCL_INLINE Tn* CreateObjectInternal() {
  Size size = SizeOf<Tn>();
  Tn * address = static_cast<Tn*> ( KCL_ALLOCATE( size ) );
  if (address) {
    Construct(address);
  }
  return address;
}
template <typename Tn> KCL_POINTER(Tn) CreateObject() {
  Tn * address = CreateObjectInternal<Tn>();
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return CreatePointer(address, address);
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return CreatePointer(address);
#   endif // KCL_MEMORY_DEBUG_ACCESS
}

template <typename Tn> void DeleteObject(KCL_POINTER(Tn) what_delete) {
  if (what_delete) {
    Destruct(what_delete);
    void * reference = static_cast<void*>( GetAddressFromPointer(what_delete) );
    KCL_DEALLOCATE(  const_cast<const void *>( reference )  );
  }
}

#ifdef    KCL_MEMORY_DEBUG
KCL_INLINE void RegisterAllocatedMemory(void * address, const Char * file, int line) {
  if (address == NULL) return;
  void * & key = address;
  MemoryMap::value_type val;
  MemoryMap::TypeInsertReturn insert_return;
  MemoryMap::Element & second = val.second;
  second.line = line;
  second.file = file;
  second.is_deleted = 0;
  second.address = address;
  val.first = key;
  ReadLockDebugMemoryLocker(); {
    bool is_new_address = g_memory_map.IsNewAddress(key);
    if (!is_new_address) {
      Char * string_buffer = (Char *) KCL_ALLOCATE(500);
      sprintf(string_buffer, "New object:: twice allocated address, possible heap destroyed!!! address = %p;  at file = %s, line = %d; ", address, file, line);
      KCL_ASSERT(0, string_buffer);
      KCL_DEALLOCATE(string_buffer);
    }
  } UpgradeDebugMemoryLocker(); {
    g_memory_map.insert(val);
  } WriteUnlockDebugMemoryLocker();
}

KCL_INLINE void UnregisterAllocatedMemory(void * address, const Char * file, int line, bool & is_correct) {
  is_correct = 1;
  if (!address) return;
  void * & pointer = address;
  MemoryMap::iterator data = (MemoryMap::iterator)NULL;

  ReadLockDebugMemoryLocker(); {
    if ( g_memory_map.count(pointer) ) {
      data = g_memory_map.find(pointer);
      bool is_deleted = data->is_deleted;
      if (is_deleted) {
        Char * string_buffer = (Char *) KCL_ALLOCATE(500);
        is_correct = 0;
        sprintf(string_buffer, "Debug memory control: double deletion!!! address = %p;  at file = %s, line = %d;  allocated at: file = %s, line = %d; first deleted at: file = %s, line = %d", address, file, line, data->file, data->line, data->file_of_delete, data->line_of_delete);
        KCL_ASSERT(0, string_buffer);
        KCL_DEALLOCATE(string_buffer);
      } else {
        data->file_of_delete = file;
        data->line_of_delete = line;
      }
    } else {
      Char * string_buffer = (Char *) KCL_ALLOCATE(500);
      is_correct = 0;
      KCL_ASSERT(0, string_buffer)
      KCL_DEALLOCATE(string_buffer);
    }
  } UpgradeDebugMemoryLocker(); {
    if (data != NULL) {
      data->is_deleted = 1;
    }
  } WriteUnlockDebugMemoryLocker();
}

template <typename Tn> KCL_POINTER(Tn) CreateObjectDebug(const Char * file, int line) {
  KCL_POINTER(Tn) re = CreateObject<Tn>();
  RegisterAllocatedMemory( GetAddressFromPointer(re), file, line);
  return re;
}

template <typename Tn> void DeleteObjectDebug(KCL_POINTER(Tn) what_delete, const Char * file, int line) {
  bool allow;
  UnregisterAllocatedMemory(  GetAddressFromPointer(what_delete), file, line, allow  );
  if (allow) {
    DeleteObject( what_delete );
  }
}
#else  // KCL_MEMORY_DEBUG
#endif // KCL_MEMORY_DEBUG

template <typename Tn_in, typename Tn_out> 
KCL_POINTER(Tn_out) UpperCastPointer( KCL_POINTER(Tn_in) in) {

#   ifdef    KCL_MEMORY_DEBUG_ACCESS
  return in.template UpperCast<Tn_out>();
#   else  // KCL_MEMORY_DEBUG_ACCESS
  return (KCL_POINTER(Tn_out)) in;
#   endif // KCL_MEMORY_DEBUG_ACCESS
}

////////////////////////////////////////////////////////////////////////////////
///  StandardMemoryPolicy
////////////////////////////////////////////////////////////////////////////////
template <typename Tn>
struct StandardMemoryPolicy {
  typedef Tn Type;
  typedef Allocator<Type> AllocatorType;
  typedef KCL_POINTER(Type) StrongPointer;
  typedef CONST_KCL_POINTER(Type) StrongConstPointer;
  typedef KCL_WEAK_POINTER(Type) WeakPointer;
  typedef CONST_KCL_WEAK_POINTER(Type) WeakConstPointer;

  KCL_FINLINE static StrongPointer GetNullStrong() {return KCL_NAMESPACE::GetNullStrong<Type>();}
  KCL_FINLINE static StrongPointer GetNullWeak() {return KCL_NAMESPACE::GetNullWeak<Type>();}
  KCL_FINLINE static StrongPointer      UnconstCastPointer(StrongConstPointer p) {return KCL_NAMESPACE::UnconstCastPointer(p);}
  KCL_FINLINE static StrongConstPointer ConstCastPointer  (StrongPointer      p) {return KCL_NAMESPACE::ConstCastPointer(p);}

  template <typename LocalType>
  struct rebind { typedef StandardMemoryPolicy<LocalType> other; };
};
 
#   endif //__cplusplus 

#endif  // KC_LIBRARY_PROTECT_OFF
