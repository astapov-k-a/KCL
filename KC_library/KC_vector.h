#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF

#   ifdef    KCL_MEMORY_DEBUG
#   define KCL_CREATE_ARRAY(ObjectType, number_elements) KCL_NAMESPACE_PERFIX CreateArrayDebug<ObjectType>(number_elements, __FILE__, __LINE__)
#   define KCL_DELETE_ARRAY(pointer)    KCL_NAMESPACE_PERFIX DeleteArrayDebug(pointer, __FILE__, __LINE__)
#   else  // KCL_MEMORY_DEBUG
#   define KCL_CREATE_ARRAY(ObjectType, number_elements) KCL_NAMESPACE_PERFIX CreateArray<ObjectType>(number_elements)
#   define KCL_DELETE_ARRAY(pointer)    KCL_NAMESPACE_PERFIX DeleteArray(pointer)
#   endif // KCL_MEMORY_DEBUG

#   ifdef    KCL_MEMORY_DEBUG
#   else  // KCL_MEMORY_DEBUG
#   endif // KCL_MEMORY_DEBUG

#   ifdef   __cplusplus
                       
template <typename Tn> class ArrayPointer;
template <typename Tn> ArrayPointer<Tn> CreateArray(Size number_elements);
template <typename Tn> void DeleteArray(ArrayPointer<Tn> what_delete);

//@class ArrayPointer

#pragma pack(push,1)
template <typename Tn> class ArrayPointer {
 public:
  typedef Tn Type;
  typedef ArrayPointer<Type> ThisT;

  ArrayPointer()   :  data_((Byte*)NULL) {}
  Size number_elements() const {
      return byte_ptr() ? (  *static_cast<const Size *>( block_address() )  ) : (0) ;}
  Type const * data() const { return reinterpret_cast<Type *>(data_); }
  Type       * data()       { return reinterpret_cast<Type *>(data_); }
  void Set(void * new_data) {data_ = new_data;}
  Type const * operator+(int index) const {return  data() + index;}
  Type       * operator+(int index)       {return  data() + index;}
  Type const * operator+(Size index) const {return  data() + index;}
  Type       * operator+(Size index)       {return  data() + index;}
  Type const & operator[](int index) const {return data()[index];}
  Type const * operator*() const {return data();}
  Type       * operator*()       {return data();}
  operator bool() {return data() != NULL; }
  void * GetMemoryBlock() {return block_address();}
  

 protected:
  const ThisT * ConstThis() {return this;}
  ArrayPointer(void * pointer) {data_ = static_cast<Byte *>(pointer);}
  Byte const * byte_ptr() const {return reinterpret_cast<const Byte *>( data() );}
  Byte       * byte_ptr()       {return const_cast<Byte *>( ConstThis() -> byte_ptr() );}
  void const * block_address() const { return static_cast<const void *>(byte_ptr()-sizeof(Size)); }
  void       * block_address()       { return const_cast<void *>( ConstThis() -> block_address() ); }
  friend ThisT CreateArray<Type>(Size);
  friend void DeleteArray<Type>(ArrayPointer<Type> what_delete);
#   ifdef    KCL_MEMORY_DEBUG
  friend ThisT CreateArrayDebug<Type>(Size, const Char * file, int line);
  friend void DeleteArrayDebug<Type>(ArrayPointer<Type> what_delete, const Char * file, int line);
#   endif // KCL_MEMORY_DEBUG

 private:
  Byte * data_;
};
#pragma pack(pop)
                                          


template <typename Tn> KCL_INLINE void InitializeArray(Size number_elements, Byte * memory_block) {
  Tn * address = static_cast<Tn *>( memory_block + sizeof(Size) );
  (reinterpret_cast<Size *>(memory_block))[0] = number_elements;
  Tn * base = address + number_elements;
  Size counter = 0 - number_elements;
  for (;counter; ++counter) {
    Construct(base + counter);
  }
}


template <typename Tn> void KCL_INLINE DeinitializeArray(ArrayPointer<Tn> what_deinit) {
  Size number_elements = what_deinit.number_elements();
  Tn * base = what_deinit.data() + number_elements;
  Size counter = 0 - number_elements;
  for (;counter; ++counter) {
    Destruct( reinterpret_cast<Tn &>(base[counter]) );
  }
}

#   ifdef    KCL_MEMORY_DEBUG
template <typename Tn> ArrayPointer<Tn> CreateArrayDebug(Size number_elements, const Char * file, int line) {
  Byte * memory_block = static_cast<Byte *> ( KCL_ALLOCATE( sizeof(Tn) * number_elements + sizeof(Size) ) );
  if (memory_block) {
    RegisterAllocatedMemory(memory_block, file, line);
    InitializeArray<Tn>(number_elements, memory_block);
    return ArrayPointer<Tn>(memory_block + sizeof(Size));
  } else {
    return ArrayPointer<Tn>();
  }
}

template <typename Tn> void DeleteArrayDebug(ArrayPointer<Tn> what_delete, const Char * file, int line) {
  if (what_delete) {
    bool allow;
    UnregisterAllocatedMemory( what_delete.block_address(), file, line, allow );
    DeinitializeArray<Tn>(what_delete);
    DeallocateOrDie( what_delete.block_address() );
  }
}
#   endif // KCL_MEMORY_DEBUG

template <typename Tn> ArrayPointer<Tn> CreateArray(Size number_elements) {
  Byte * memory_block = static_cast<Byte *> ( KCL_ALLOCATE( sizeof(Tn)*number_elements + sizeof(Size) ) );
  DiagnosticPrint(KERN_EMERG "Universal CreateArray started, memory_block = %p",memory_block);
  if (memory_block) {
    InitializeArray<Tn>(number_elements, memory_block);
    return ArrayPointer<Tn>(memory_block + sizeof(Size));
  } else {
    return ArrayPointer<Tn>();
  }
}

template <typename Tn> void DeleteArray(ArrayPointer<Tn> what_delete) {
  if (what_delete) {
    DeinitializeArray<Tn>(what_delete);
    KCL_DEALLOCATE( what_delete.block_address() );
  }
}

template <typename Tn> class Vector {
 public:
  typedef Tn Type;
  typedef Vector<Tn> ThisT;
  typedef KCL_ARRAY_POINTER(Type)      StrongPointer;
  typedef KCL_ARRAY_POINTER(Type) ConstStrongPointer;
  typedef Type * iterator;
  typedef Type const * const_iterator;

  enum { kMinReservedElements = 8, };

  Vector()    :  size_(0) {}
  ~Vector() { DeleteData();}
  bool Initialize(Size number_elements);
  bool reserve(Size number_elements);
  bool resize(Size number_elements);
  Type const & operator[](int index) const;
  Type       & operator[](int index);
  Type const & at(int index) const;
  Type       & at(int index);
  void push_back(const Type & to_add);
  void push_back();
  void pop_back();
  Type const & back() const;
  Type       & back();
  bool empty() {return size() == NULL;}
  const_iterator begin() const {return array();}
  const_iterator end() const {return postend();}
  iterator begin() {return array();}
  iterator end()   {return postend();}
  Size size() const {return data().number_elements();}

 protected:
  Size capacity() const;
  Type * postend() const;
  void postend(Type *);
  StrongPointer      data() {return data_;}
  ConstStrongPointer data() const {return data_;}
  void data(StrongPointer value) {data_ = value;}
  bool IsDataNull() const {return !data();}
  Type const * array() const {return data().data();}
  Type       * array()       {return data().data();}
  void Push_();
  void Pop_();

  StrongPointer AllocateArray(Size number_elements);
  void FreeArray(StrongPointer to_delete);
  bool CreateData(Size number_elements);
  void DeleteData();
  static void CopyElementsPostend( iterator source_postend, iterator destination_postend, Size number_elements );
  static void CopyElements( iterator source, iterator destination, Size number_elements );
  static Size GetUpper2exp(Size value);
  static Size GetLowerEqual2exp(Size value);

 private:
  Vector(const ThisT &);
  ThisT& operator=(const ThisT &);

  Size size_;
  StrongPointer  data_;
};


template <typename Tn>
bool Vector<Tn>::Initialize(Size number_elements) {
  KCL_ASSERT( !data(), "Vector::Initialize - memory leak");
  return CreateData(number_elements);
}

template <typename Tn>
bool Vector<Tn>::reserve(Size number_elements) {
  if ( number_elements > capacity() ) {
    StrongPointer new_data = AllocateArray(number_elements);
    if (!new_data) return 0;
    StrongPointer old_data = data();
    if (!IsDataNull()) {
      Size old_size = capacity();
      Type * src_postend = postend();
      Type * dst_postend = data().data() + data();
      CopyElementsPostend(src_postend, dst_postend, old_size);
      DeleteData();
    }    
    data(new_data);
  }
  return 1;
}

template <typename Tn>
bool Vector<Tn>::resize(Size number_elements) {
  Size old_capacity = capacity();
  Size old_size = size();
  if (number_elements > old_capacity ) {
    reserve(number_elements << 1);
  }
  if (old_size < number_elements) {    
    number_elements -= old_size;
    for (; number_elements; --number_elements) Push_();
  }
  return 1;
}

template <typename Tn>
Size Vector<Tn>::capacity() const {
  return IsDataNull() ? 0 : data().number_elements();
}

template <typename Tn>
typename Vector<Tn>::StrongPointer  
Vector<Tn>::AllocateArray(Size number_elements) {
  return KCL_CREATE_ARRAY(Type, number_elements);  
}

template <typename Tn>
void  Vector<Tn>::FreeArray(StrongPointer to_delete) {
  return KCL_DELETE_ARRAY(to_delete);  
}

template <typename Tn>
void  Vector<Tn>::DeleteData() {
  return FreeArray( data() );
}

template <typename Tn>
void  Vector<Tn>::CopyElementsPostend( iterator source_postend, iterator destination_postend, Size number_elements ) {
  Size current = 0 - number_elements;
  for (; current; ++current) {
    destination_postend[current] = source_postend[current];
  } 
}

template <typename Tn>
void  Vector<Tn>::CopyElements( iterator source, iterator destination, Size number_elements ) {
  CopyElementsPostend( source + number_elements, destination + number_elements, number_elements);
}

template <typename Tn>
bool  Vector<Tn>::CreateData(Size number_elements) {
  data(AllocateArray(number_elements));
  return data();
}

template <typename Tn>
Size Vector<Tn>::GetUpper2exp(Size value) {
  if (value == 0) return 1;
  Size re = 1;
  while (re <= value) re <<= 1;
  return re;
}

template <typename Tn>
Size Vector<Tn>::GetLowerEqual2exp(Size value) {
  if (value == 0) return 0;
  Size re = 1;
  while (re < value) re <<= 1;
  return re;  
}

template <typename Tn>
void Vector<Tn>::Push_() {
  Construct( array() + (size_++) );
}

template <typename Tn>
void Vector<Tn>::push_back() {
  Size new_capacity = capacity();
  KCL_ASSERT(size() <= new_capacity, "Vector::push_back : Integrity Error!!! size > capacity" );
  if ( size() == new_capacity ) {
    new_capacity = Max( kMinReservedElements, new_capacity << 1);
    reserve( new_capacity );
  }
  Push_();
}

template <typename Tn>
void Vector<Tn>::pop_back() {
  Size sz = size();
  KCL_ASSERT ( sz != 0, "Vector::pop_back : Integrity error -> empty vector");
  if (sz) {
    Pop_();
  }
}

template <typename Tn>
void Vector<Tn>::Pop_() {
  Destruct(array()[--size_]);
}

template <typename Tn>
Tn * Vector<Tn>::postend() const {
  return ( data() + size() );
}

#   endif //__cplusplus

#endif  // KC_LIBRARY_PROTECT_OFF
