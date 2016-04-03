#include "KC_library.h"
extern "C" {
#ifdef KCL_NO_KERNEL
#   ifndef  _WIN32
#      include <sys/types.h>
#   endif //_WIN32
#endif
}


#ifndef  _WIN32
#endif //_WIN32
//typedef size_t Size;
#ifdef      KCL_NO_KERNEL
#include <stdlib.h>
#endif //KCL_NO_KERNEL




#      ifdef    KCL_MEMORY_DEBUG


namespace KCL_NAMESPACE { //namespace kcl started
MemoryMap g_memory_map;
} //namespace kcl finished

extern "C" {

/// @debug
class IntHash {
 public:
  static Size Calculate(Size x) {return x;}
};

void InitializeMemoryDebugger() {

  InitializeDebugMemoryLock();
  bool is_success = g_memory_map. Initialize();
  KCL_ASSERT(is_success, "DebugMemoryMap initialization failed!!!");
}

void UninitializeMemoryDebugger() {
  WriteLockDebugMemoryLocker();
  g_memory_map.Destroy();
  WriteUnlockDebugMemoryLocker();
  UninitializeDebugMemoryLock();
}

bool FindLeaks() {

  return KCL_NAMESPACE_PERFIX g_memory_map.FindLeaks();
}

bool FindLeaksAndAlert() {
  return KCL_NAMESPACE_PERFIX g_memory_map.FindLeaksAndAlert();
}
void   GetSnapshotMemoryDebugger(void * snapshot_pointer) {
  typedef KCL_NAMESPACE_PERFIX MemoryMap::Snapshot Snapshot;
  Snapshot * snapshot = static_cast<Snapshot *>(snapshot_pointer);
  KCL_NAMESPACE_PERFIX g_memory_map.GetSnapshot(*snapshot);
}
bool IsEqualMemoryDebugger(const void * snapshot_pointer, bool need_alert) {
  typedef KCL_NAMESPACE_PERFIX MemoryMap::Snapshot Snapshot;
  const Snapshot * snapshot = static_cast<const Snapshot *>(snapshot_pointer);
  return KCL_NAMESPACE_PERFIX g_memory_map.IsEqual(*snapshot, need_alert);
}

} //extern "C"

#      else  // KCL_MEMORY_DEBUG
void InitializeMemoryDebugger() {}
void UninitializeMemoryDebugger() {}
#      endif // KCL_MEMORY_DEBUG



namespace KCL_NAMESPACE {


extern "C" {

void * Allocate(Size number_of_bytes) {
#ifdef KCL_NO_KERNEL
  void * return_address = malloc(number_of_bytes);
  return return_address;
#else
  void * return_address =  KCL_Allocate_C(number_of_bytes);
  return return_address;
#endif
}

extern void  DeallocateOrDie(const void * pointer) {
#ifdef KCL_NO_KERNEL
  free( (void*) pointer);
#else
  KCL_FreeOrDie_C( const_cast<void *>(pointer) );
#endif
}

extern void  SafeDeallocate(const void * pointer) {
#ifdef KCL_NO_KERNEL
  if (pointer) free( (void*) pointer);
#else
  if (pointer) KCL_FreeOrDie_C( const_cast<void *>(pointer) );
#endif
}

extern void * Reallocate(const void *pointer, Size number_of_bytes) {
#ifdef KCL_NO_KERNEL
  return realloc( (void*) pointer, number_of_bytes);
#else
  return KCL_Rellocate_C(pointer, number_of_bytes);
#endif
}

#ifdef KCL_NO_KERNEL
#   ifndef _WIN32
#      define RECEIVE_FUNCTIONS_ON
#   endif
#else
#   define GET_NLA_DATA_MACRO(na) ((void *)((char*)(na) + NLA_HDRLEN))
#endif


}




///@todo сделать нормальный MemoryLength, более оптимальный
Size MemoryLength(const void * pointer) {
  if (!pointer) return 0;
  const Byte * bytes  = static_cast<const Byte *>(pointer);
  PointerDifference intptr = reinterpret_cast<PointerDifference>(pointer);
  Size max_size = (0 - intptr);
  enum {kMaxRationalLength = 0xFFffFF};
  enum {kNullSym = 0};
  max_size = Min(kMaxRationalLength, max_size);
  for ( Size i = 0; i < max_size; ++i ) {
    if ( bytes[i] == kNullSym ) return i;
  }
  return kMaxRationalLength;
}


void String::Append_(const CharType * to_add, Size number_chars_to_add) {

}

void String::Resize_(Size new_size) {
  if ( IsNull() ) {
    MakeDataForThis_(new_size);
  }
}

void String::MakeDataForThis_(Size new_size) {
  data(MakeData_(new_size));
  size(new_size);
  is_owner(true);
}

typename String::CharType *
String::MakeData_(Size new_size) {
  CharType * re = static_cast<CharType *> ( KCL_ALLOCATE(new_size + kTerminatorSize) );
  KCL_ASSERT( (bool)re, "String: out of memory");
  return re;
}

void String::DeleteData_(CharType * pointer) {
  KCL_DEALLOCATE(pointer);
}

void String::DeleteDataForThis_() {
  if ( is_owner() ) DeleteData_(data());
  data(NULL);
  size(0);
  is_owner(0);
}

typename String::This
String::MakeReference(const CharType * string) {
  CharType * string_value = const_cast<CharType *> (string);
  This re;
  bool is_error;
  re.is_owner(false);
  re.data(string_value);
  re.size( GetStringLength(string_value, &is_error) );
  KCL_ASSERT(!is_error, "Creating reference String from const multibyte string failed!!! Source String not null-terminated");
  if (is_error) {
    re.size(0);
  }
  return re;
}

} //namespace kcl

KCL_EXTERN const Char * GetErrorTextForSafePointers(Size error_code, Size operator_code) {
  Size index = 0;
  static const Char * error_parameter = "Safe pointers error : invalid GetErrorTextFolrSafePointers parameters";
  static const Char * errors[] = {
    "Safe pointers error : No error",
    "Safe pointers error, operator* : Null dereferencing",
    "Safe pointers error, operator* : Overwrite",
    "Safe pointers error, operator* : Underwrite",

    "Safe pointers error, operator-> : Null dereferencing",
    "Safe pointers error, operator-> : Overwrite",
    "Safe pointers error, operator-> : Underwrite",

    "Safe pointers error, operator[] : Null dereferencing",
    "Safe pointers error, operator[] : Overwrite",
    "Safe pointers error, operator[] : Underwrite"
  };
  enum {kIndexLimit = KCL_NAMESPACE_PERFIX PointerRangeEnums::kErrorLimit * KCL_NAMESPACE_PERFIX PointerRangeEnums::kOperatorNumber};
  index = (error_code != KCL_NAMESPACE_PERFIX PointerRangeEnums::kNoError) ?
          (error_code + operator_code * (KCL_NAMESPACE_PERFIX PointerRangeEnums::kErrorLimit) ) :
          ( 0 );
  if (index > kIndexLimit) return error_parameter;
  return errors[index];
}

