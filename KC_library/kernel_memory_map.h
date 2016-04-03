#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF

#   ifdef  __cplusplus


template <typename FirstTn, typename SecondTn> class pair {
 public:
  template <typename Tn> friend class SafePointer;
  typedef FirstTn First;
  typedef SecondTn Second;
  pair()    :  first(), second() {}

  pair(const FirstTn & val1, const SecondTn & val2)
      :  first(val1), second(val2) {  }

  pair(First & val1, Second & val2)
      :  first(val1), second(val2) {  }

//  pair(First & val1, Second & val2) {
//    first = val1;
//    second = val2;
//  }
//  pair(const First & val1, const Second & val2) : pair() {
//    first = val1;
//    second = val2;
//  }

  First first;
  Second second;
};


#      ifdef    KCL_MEMORY_DEBUG

class MemoryMap {

 public:

  struct Element {
    int line;
    const Char * file;
    bool is_deleted;
    void * address;
    const Char * file_of_delete;
    int line_of_delete;
  };
  typedef MemoryMap ThisT;
  typedef void * MapKey;
  typedef UInteger32 HashType;
  typedef const Element * ConstIterator;
  typedef       Element *      Iterator;
  typedef pair <MapKey, Element> ValueType;
  class Snapshot;

  //stl-like
  typedef MapKey key_type;
  typedef ConstIterator const_iterator;
  typedef Iterator iterator;
  typedef Size size_type;
  typedef Element mapped_type;
  typedef ValueType value_type;

  typedef pair<iterator, bool> TypeInsertReturn;

 protected:
  HashType GetElementHash(const Element & arg) {
    //UInteger64  int_val = (UInteger64) arg.address >> 12;
    UInteger64  int_val = (UInteger64) arg.address;
    UInteger32 hash = (UInteger32)((int_val)>>33^(int_val)^(int_val)<<11);
    return hash;
  }
  static HashType GetKeyHash(const MapKey & arg) {
    UInteger64  int_val = (UInteger64) arg;
    UInteger32 hash = (UInteger32)((int_val)>>33^(int_val)^(int_val)<<11);
    return hash;
//#define kh_int64_hash_func(key) (khint32_t)((key)>>33^(key)^(key)<<11)
  }
  static bool IsHashesEqual(const HashType arg1, const HashType arg2) {
    return arg1 == arg2;
  }
  static bool IsHashesEqual(const MapKey arg1, const MapKey arg2) {
    return arg1 == arg2;
  }

 public:
  MemoryMap()  {}
  //~MemoryMap() {Destroy_();}
  bool Initialize() {return Initialize_();}
  void Destroy() {Destroy_();}
  Size count (const MapKey& k) const {return Count_(k);}
  bool IsNewAddress (const MapKey& k) const {return IsNewAddress_(k);}
  Iterator      find (const MapKey& k) {return Find_(k);}
  ConstIterator find (const MapKey& k) const {return Find_(k);}
  void clear() {Clear_();}
  TypeInsertReturn insert (const ValueType& val) {return Insert_(val);}
  Size erase (const MapKey& k) {return Erase_(k);}
  //void erase (Iterator position) {Erase_(position);}
  Element& operator[] (const MapKey& k) {return Get_(k);}
  Iterator begin() {return begin_();}
  Iterator end() {return end_();}//*/
  bool GetSnapshot(Snapshot & out_snapshot) const {return GetSnapshot_(out_snapshot);}
  bool FindLeaks() const {return FindLeaks_();}
  bool IsEqual(const Snapshot & with, bool need_alert) const {return IsEqual_(with, need_alert);}
  bool FindLeaksAndAlert() const {return FindLeaksAndAlert_();}
  void * DebugHash() {return DebugHash_();}
};
#      endif // KCL_MEMORY_DEBUG

#   endif //__cplusplus

#endif  // KC_LIBRARY_PROTECT_OFF
