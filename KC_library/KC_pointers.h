#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF

#ifdef    __cplusplus

template <typename Tn> class BasePointer;
template <typename Tn> class BaseConstPointer;
template <typename Tn> class SafePointer;
template <typename Tn> class SafeConstPointer;

namespace internal {
////////////////////////////////////////////////////////////////////////////////

template< class T > struct DereferenceTypeFor
{
    typedef T & type;
};
template<> struct DereferenceTypeFor< void >
{
    typedef void type;
};
template<> struct DereferenceTypeFor< void const >
{
    typedef void type;
};
template<> struct DereferenceTypeFor< void volatile >
{
    typedef void type;
};
template<> struct DereferenceTypeFor< void const volatile >
{
    typedef void type;
};

template< class T > struct DereferenceTypeFor< T[] >
{
    typedef void type;
};
template< class T, Size N > struct DereferenceTypeFor< T[N] >
{
    typedef void type;
};

template< class T > struct AccessTypeFor
{
    typedef T * type;
};

template< class T > struct AccessTypeFor< T[] >
{
    typedef void type;
};

template< class T, Size N > struct AccessTypeFor< T[N] >
{
    typedef void type;
};


////////////////////////////////////////////////////////////////////////////////
} // internal namespace

template <typename Tn> class AbstractBasePointer {
 public:
  typedef Tn Type;
  typedef AbstractBasePointer<Type> ThisT;
  typedef BaseConstPointer<Type> ConstPointerT;
  AbstractBasePointer(const ThisT& value)   :  pointer_(value.pointer_) {}
  AbstractBasePointer()    :  pointer_((Type*)NULL) {}
  explicit AbstractBasePointer(Type * value)    :  pointer_(value) {
  }
  explicit AbstractBasePointer(int value)    :  pointer_((Type*)value) {
    KCL_ASSERT(value == 0, "BasePointer constructor: not null integer assigment");
  }
  KCL_FINLINE ThisT & operator=(int value);
  KCL_FINLINE ThisT & operator=(Type * value);

  KCL_FINLINE static ThisT GetNullObject() {return ThisT();}
  KCL_FINLINE ConstPointerT ConstCast() const {return ConstPointerT( get_pointer() ); }
  template <typename ToCast>        KCL_FINLINE BasePointer<ToCast> UpperCast() {return BasePointer<ToCast>((ToCast*)get_pointer());}
  template <typename ToCast> static KCL_FINLINE BasePointer<ToCast> UpperCast(ThisT& from_cast) {return BasePointer<ToCast>( (ToCast*)from_cast.get_pointer());}

  KCL_FINLINE Type       * operator->() const {return  get_pointer();}
  KCL_FINLINE Type       * operator[](int index) const {return  get_pointer()[index];}

  KCL_FINLINE operator         bool() const {
    return pointer() != NULL;
  }

  KCL_FINLINE bool operator==(Type * value) const;
  KCL_FINLINE bool operator!=(Type * value) const;
  KCL_FINLINE bool operator==(const ThisT & value) const;
  KCL_FINLINE bool operator!=(const ThisT & value) const;

  KCL_FINLINE Type const * Data() const {return     pointer();}
  KCL_FINLINE Type       * GetData()    {return get_pointer();}

 protected:
  template <typename Type2> friend class AbstractSafePointer;
  KCL_FINLINE Type const *     pointer() const {return pointer_;}
  KCL_FINLINE Type       * get_pointer() const {return pointer_;}
  KCL_FINLINE void pointer(Type * value) {pointer_ = value;}

 private:
  Type * pointer_;
};

template <> class BasePointer<void> : public AbstractBasePointer<void> {
};

template <typename Tn> class BasePointer : public AbstractBasePointer<Tn> {
 public:
  typedef Tn Type;
  typedef BasePointer<Type> ThisT;
  typedef BaseConstPointer<Type> ConstPointerT;
  typedef AbstractBasePointer<Type> Parent;
  KCL_FINLINE BasePointer(const ThisT& value)   :  Parent(value) {}
  KCL_FINLINE BasePointer()    :  Parent((Type*)NULL) {}
  KCL_FINLINE explicit BasePointer(Type * value)    :  Parent(value) {}
  KCL_FINLINE explicit BasePointer(void * value)    :  Parent((Type*)value) {}
  KCL_FINLINE explicit BasePointer(int value)    :  Parent((Type*)value) {  }

  KCL_FINLINE ThisT & operator=(void * value);
  KCL_FINLINE Type       & operator*()  const {return *get_pointer();}
  KCL_FINLINE bool operator==(void * value) const;
  KCL_FINLINE bool operator!=(void * value) const;
  KCL_FINLINE static ThisT GetNullObject() {return ThisT();}

protected:
  using Parent::get_pointer;
  using Parent::pointer;
};

template <typename Tn> class AbstractBaseConstPointer : public AbstractBasePointer<Tn> {
 public:
  typedef Tn Type;
  typedef AbstractBaseConstPointer<Type> ThisT;
  typedef AbstractBasePointer<Type> ParentT;
  typedef BasePointer<Type> PointerT;

  KCL_FINLINE AbstractBaseConstPointer(const ThisT& value)   :  ParentT(value) {}
  KCL_FINLINE AbstractBaseConstPointer()    :  ParentT() {}
  KCL_FINLINE explicit AbstractBaseConstPointer(Type * value)    :  ParentT(value) {}
  KCL_FINLINE ThisT& operator=(void * value)    {return static_cast<ThisT&> (  ParentT::operator=((Type*)value) );}
  KCL_FINLINE ThisT& operator=(const ThisT & value)    {return operator=( value.get_pointer() );}

  KCL_FINLINE static ThisT GetNullObject() {return ThisT();}
  KCL_FINLINE PointerT Unconst() const {return PointerT (get_pointer());}
  template <typename ToCast>        KCL_FINLINE AbstractBaseConstPointer<ToCast> UpperCast() {return AbstractBaseConstPointer<ToCast>( (ToCast*)get_pointer());}
  template <typename ToCast> static KCL_FINLINE AbstractBaseConstPointer<ToCast> UpperCast(ThisT& from_cast) {return AbstractBaseConstPointer<ToCast>( (ToCast*)from_cast.get_pointer());}


  KCL_FINLINE Type const * operator->() const {return     pointer();}
  KCL_FINLINE Type const * operator[](int index) const {return  pointer()[index];}

 protected:
  using ParentT::get_pointer;
  using ParentT::pointer;

 private:
  template <typename Type2> friend class AbstractSafeConstPointer;
  template <typename Type2> friend class BaseConstPointer;

  Type       *    GetData()      ;
};


template <> class BaseConstPointer<void> : public AbstractBaseConstPointer<void> {
 public:
  typedef void Type;
  typedef BaseConstPointer<Type> ThisT;
  typedef AbstractBaseConstPointer<Type> ParentT;
  typedef BasePointer<Type> PointerT;
  KCL_FINLINE explicit BaseConstPointer(void * value)    :  ParentT(value) {}
};


template <typename Tn> class BaseConstPointer : public AbstractBaseConstPointer<Tn> {
 public:
  typedef Tn Type;
  typedef BaseConstPointer<Type> ThisT;
  typedef AbstractBaseConstPointer<Type> ParentT;
  typedef BasePointer<Type> PointerT;

  KCL_FINLINE BaseConstPointer(const ThisT& value)   :  ParentT(value) {}
  KCL_FINLINE BaseConstPointer()    :  ParentT() {}
  KCL_FINLINE explicit BaseConstPointer(Type * value)    :  ParentT(value) {}
  KCL_FINLINE explicit BaseConstPointer(void * value)    :  ParentT((Type*)value) {}
  KCL_FINLINE ThisT& operator=(void * value)    {return static_cast<ThisT&> (  ParentT::operator=((Type*)value) );}
  KCL_FINLINE ThisT& operator=(const ThisT & value)    {return operator=( value.get_pointer() );}

  KCL_FINLINE static ThisT GetNullObject() {return ThisT();}
  using ParentT::Unconst;
  using ParentT::UpperCast;

  KCL_FINLINE Type const & operator*()  const {return *   pointer();}

 protected:
  using ParentT::get_pointer;
  using ParentT::pointer;
};

class PointerRangeEnums {
 public:
  enum ErrorsEnum {
    kNoError = 0,
    kErrorNull = 1,
    kErrorOverwrite = 2,
    kErrorUnderwrite = 3,
    kErrorNumber,
    kErrorPostEnd =kErrorNumber ,
    kErrorLimit = kErrorPostEnd - 1
  } ;
  enum OperatorsEnum {
    kOperatorDereferencing = 0,
    kOperatorAccess = 1,
    kOperatorIndexation = 2,
    kOperatorNumber,
    kOperatorLimit = kOperatorNumber -1
  };
};

template <typename Tn> class PointerRange : public PointerRangeEnums {
 public:
  typedef Tn Type;
  typedef PointerRange<Type> ThisT;
  typedef PointerRangeEnums Enums;
  using Enums::ErrorsEnum;
  using Enums::OperatorsEnum;
  KCL_FINLINE PointerRange(const void * low_limit_arg, const void * high_limit_arg)
      :  low_limit_(low_limit_arg),
         high_limit_(high_limit_arg) {}

 protected:
  bool KCL_FINLINE  Verify(const Tn * pointer_arg, Size & out_error_code) const;
  bool KCL_FINLINE  Verify(const Tn * pointer_arg, Size length_in_bytes, Size & out_error_code) const;

  KCL_FINLINE void const *      low_limit() const {return         low_limit_;}
  KCL_FINLINE void       *  get_low_limit() const {return (void *)low_limit_;}
  KCL_FINLINE void low_limit(void * value) {low_limit_ = value;}
  KCL_FINLINE void const *       high_limit() const {return          high_limit_;}
  KCL_FINLINE void       *   get_high_limit() const {return  (void *)high_limit_;}
  KCL_FINLINE void high_limit(void * value)  {high_limit_ = value;}

 private:
  const void * low_limit_;
  const void *  high_limit_;
};

template <typename Tn> class AbstractSafePointer
    :  public BasePointer<Tn>,
       protected PointerRange<Tn>
{
 public:
  typedef Tn Type;
  typedef AbstractSafePointer<Type> ThisT;
  typedef BasePointer<Type> ParentT;
  typedef PointerRange<Type> RangeT;
  typedef SafeConstPointer<Type> ConstPointerT;
  typedef typename RangeT::ErrorsEnum  ErrorsEnum;
  typedef typename RangeT::OperatorsEnum OperatorsEnum;
  using ParentT::operator bool;
  using ParentT::operator==;
  using ParentT::operator!=;
  using ParentT::GetData;

  KCL_FINLINE AbstractSafePointer(
      Type * pointer_arg,
      const void * low_limit_arg,
      const void * high_limit_arg)
      :  ParentT(pointer_arg),
         RangeT(low_limit_arg, high_limit_arg) {
  }
  KCL_FINLINE AbstractSafePointer(const ThisT& value)
      :  ParentT(value.get_pointer()),
         RangeT(value.get_low_limit(), value.get_high_limit()) {
  }
  KCL_FINLINE ThisT& operator=(const ThisT& value);

  KCL_FINLINE Type const * operator->() const;
  KCL_FINLINE Type       * operator->();
  KCL_FINLINE Type       * operator[](int index);


  KCL_FINLINE static ThisT GetNullObject() {return ThisT();}
  KCL_FINLINE ConstPointerT ConstCast() const {return ConstPointerT( get_pointer() , get_low_limit(), get_high_limit() ); }
  template <typename ToCast>        KCL_FINLINE AbstractSafePointer<ToCast> UpperCast() {return AbstractSafePointer<ToCast>( (ToCast*)get_pointer(), get_low_limit(), get_high_limit());}
  template <typename ToCast> static KCL_FINLINE AbstractSafePointer<ToCast> UpperCast(ThisT& from_cast) {return AbstractSafePointer<ToCast>( (ToCast*)from_cast.get_pointer(), from_cast.get_low_limit(), from_cast.get_high_limit());}

 protected:
  KCL_FINLINE AbstractSafePointer()    :  ParentT((void*)NULL), RangeT(NULL, NULL) {
    return;
  }

  using ParentT::pointer;
  using ParentT::get_pointer;
  using RangeT::Verify;
  using RangeT::get_low_limit;
  using RangeT::low_limit;
  using RangeT::get_high_limit;
  using RangeT::high_limit;
  KCL_FINLINE bool Verify(Size & out_error_code) const {return this->Verify(pointer(),out_error_code);}

 private:
};


template <typename Tn> class AbstractSafeConstPointer
    :  public AbstractBaseConstPointer<Tn>,
       protected PointerRange<Tn> {
 public:
  typedef Tn Type;
  typedef AbstractSafeConstPointer<Type> ThisT;
  typedef AbstractBaseConstPointer<Type> ParentT;
  typedef PointerRange<Type> RangeT;
  typedef AbstractSafePointer<Type> PointerT;
  typedef typename RangeT::ErrorsEnum  ErrorsEnum;
  typedef typename RangeT::OperatorsEnum OperatorsEnum;
  using ParentT::operator bool;
  using ParentT::Data;

  KCL_FINLINE AbstractSafeConstPointer(
      Type * pointer_arg,
      const void * low_limit_arg,
      const void * high_limit_arg)
      :  ParentT(pointer_arg),
         RangeT(low_limit_arg, high_limit_arg) {}
  KCL_FINLINE AbstractSafeConstPointer(const ThisT& value)
      :  ParentT((Type*)value.get_pointer()),
         RangeT(value.get_low_limit(), value.get_high_limit()) {}
  KCL_FINLINE ThisT& operator=(const ThisT& value);


  KCL_FINLINE Type const * operator->() const;
  KCL_FINLINE Type const * operator[](int index) const;


  KCL_FINLINE static ThisT GetNullObject() {return ThisT();}
  KCL_FINLINE PointerT Unconst() const {return PointerT (get_pointer() , get_low_limit(), get_high_limit() );}
  template <typename ToCast>        KCL_FINLINE AbstractSafeConstPointer<ToCast> UpperCast() {return AbstractSafeConstPointer<ToCast>( (ToCast*)get_pointer(), get_low_limit(), get_high_limit());}
  template <typename ToCast> static KCL_FINLINE AbstractSafeConstPointer<ToCast> UpperCast(ThisT& from_cast) {return AbstractSafeConstPointer<ToCast>( (ToCast*)from_cast.get_pointer(), from_cast.get_low_limit(), from_cast.get_high_limit());}

 protected:
  KCL_FINLINE AbstractSafeConstPointer()    :  ParentT( ParentT::GetNullObject() ), RangeT(NULL, NULL) {
  }

  using ParentT::pointer;
  using ParentT::get_pointer;
  using RangeT::Verify;
  using RangeT::get_low_limit;
  using RangeT::low_limit;
  using RangeT::get_high_limit;
  using RangeT::high_limit;
  bool Verify(Size & out_error_code) const {return this->Verify(pointer(),out_error_code);}

 private:
};


template <> class SafePointer<void>
    :  public AbstractSafePointer<void> {
};

template <typename Tn> class SafePointer
    :  public AbstractSafePointer<Tn>
{
 public:
  typedef Tn Type;
  typedef SafePointer<Type> ThisT;
  typedef AbstractSafePointer<Type> ParentT;
  typedef PointerRange<Type> RangeT;
  typedef SafeConstPointer<Type> ConstPointerT;
  typedef typename RangeT::ErrorsEnum  ErrorsEnum;
  typedef typename RangeT::OperatorsEnum OperatorsEnum;
  using ParentT::operator bool;
  using ParentT::operator==;
  using ParentT::operator!=;
  using ParentT::GetData;

  KCL_FINLINE SafePointer()    :  ParentT() {} ///@debug после отладки перенести в protected
  KCL_FINLINE SafePointer(
      Type * pointer_arg,
      const void * low_limit_arg,
      const void * high_limit_arg)
      :  ParentT(pointer_arg, low_limit_arg, high_limit_arg) {}
  SafePointer(const ThisT& value)
      :  ParentT(value) {
    static volatile int debug_x = 0;  /// @debug
    ++debug_x; /// @debug
  }


  KCL_FINLINE ThisT& operator=(const ThisT & value)    {
    return static_cast<ThisT&>(  ParentT::operator=( static_cast<const ParentT&>(value) )  );
  }

  KCL_FINLINE bool operator!=(const ThisT & arg) const {return AbstractBasePointer<Type>::operator !=( (AbstractBasePointer<Type>&)arg );}
  KCL_FINLINE bool operator==(const ThisT & arg) const {return AbstractBasePointer<Type>::operator ==( (AbstractBasePointer<Type>&)arg );}

  KCL_FINLINE Type const & operator*() const;
  KCL_FINLINE Type       & operator*();
  KCL_FINLINE Type       & operator[](int index);


  KCL_FINLINE static ThisT GetNullObject() {return ThisT();}
  using ParentT::ConstCast;
  using ParentT::UpperCast;

 protected:

  using ParentT::pointer;
  using ParentT::get_pointer;
  using RangeT::Verify;
  using RangeT::get_low_limit;
  using RangeT::low_limit;
  using RangeT::get_high_limit;
  using RangeT::high_limit;
  KCL_FINLINE bool Verify(Size & out_error_code) const {return this->Verify(pointer(),out_error_code);}

 private:
};

template <> class SafeConstPointer<void>
    :  public AbstractSafeConstPointer<void> {
};

template <typename Tn> class SafeConstPointer
    :  public AbstractSafeConstPointer<Tn> {
 public:
  typedef Tn Type;
  typedef SafeConstPointer<Type> ThisT;
  typedef AbstractSafeConstPointer<Type> ParentT;
  typedef PointerRange<Type> RangeT;
  typedef SafePointer<Type> PointerT;
  typedef typename RangeT::ErrorsEnum  ErrorsEnum;
  typedef typename RangeT::OperatorsEnum OperatorsEnum;
  using ParentT::operator bool;
  KCL_FINLINE bool operator!=(const ThisT & arg) const {return AbstractBasePointer<Type>::operator !=( (AbstractBasePointer<Type>&)arg );}
  KCL_FINLINE bool operator==(const ThisT & arg) const {return AbstractBasePointer<Type>::operator ==( (AbstractBasePointer<Type>&)arg );}
  using ParentT::Data;

  KCL_FINLINE SafeConstPointer(
      Type * pointer_arg,
      const void * low_limit_arg,
      const void * high_limit_arg)
      :  ParentT(pointer_arg, low_limit_arg, high_limit_arg) {}
  KCL_FINLINE SafeConstPointer(const ThisT& value)
      :  ParentT(value) {}

  KCL_FINLINE ThisT& operator=(const ThisT & value)    {return static_cast<ThisT&>(  ParentT::operator=( static_cast<const ParentT&>(value) )  );}

  KCL_FINLINE Type const & operator*() const;
  KCL_FINLINE Type const & operator[](int index) const;


  KCL_FINLINE static ThisT GetNullObject() {return ThisT();}
  using ParentT::Unconst;
  using ParentT::UpperCast;

 protected:
  KCL_FINLINE SafeConstPointer()    :  ParentT(  ) {
  }

  using ParentT::pointer;
  using ParentT::get_pointer;
  using RangeT::Verify;
  using RangeT::get_low_limit;
  using RangeT::low_limit;
  using RangeT::get_high_limit;
  using RangeT::high_limit;
  KCL_FINLINE bool Verify(Size & out_error_code) const {return this->Verify(pointer(),out_error_code);}

 private:
};

template <typename Tn>
typename AbstractBasePointer<Tn>::ThisT & AbstractBasePointer<Tn>::operator=(int value)  {
  KCL_ASSERT(value == 0, "BasePointer::operartor= assigment not null integer!!!");
  pointer((Type*)value);
}

template <typename Tn>
typename BasePointer<Tn>::ThisT & BasePointer<Tn>::operator=(void * value)  {
  pointer((Type*)value);
}

template <typename Tn>
typename AbstractBasePointer<Tn>::ThisT & AbstractBasePointer<Tn>::operator=(Type * value)  {
  pointer(value);
}

template <typename Tn>
bool AbstractBasePointer<Tn>::operator==(Type * value) const {
  return pointer() == (const Type *)value;
}

template <typename Tn>
bool AbstractBasePointer<Tn>::operator!=(Type * value) const {
  return pointer() != (const Type *)value;
}

template <typename Tn>
bool AbstractBasePointer<Tn>::operator==(const ThisT & value) const {
  return pointer() == value.pointer();
}

template <typename Tn>
bool AbstractBasePointer<Tn>::operator!=(const ThisT & value) const {
  return pointer() != value.pointer();
}

template <typename Tn>
bool inline PointerRange<Tn>::Verify(const Type * pointer_arg, Size & out_error_code) const {
    return Verify(pointer_arg, sizeof(Tn), out_error_code);
}

template <typename Tn>
bool inline PointerRange<Tn>::Verify(const Tn * pointer_arg, Size length_in_bytes, Size & out_error_code) const {
  const Byte * data_limit = reinterpret_cast<const Byte *>(pointer_arg) + length_in_bytes - 1;
  if ( pointer_arg == (Type*)NULL ) {
    DiagnosticPrint(KERN_EMERG "null pointer dereferencing: pointer = %p, dn = %p, up = %p, size = %lu, up-dn = %lu, data_last_byte = %p, size_parameter = %lu", pointer_arg, low_limit(), high_limit(), KCL_NAMESPACE_PERFIX SizeOf<Type>(), (Byte*)high_limit() - (Byte*)low_limit(), data_limit, length_in_bytes );
    out_error_code = kErrorNull;
    return 0;
  }
  KCL_ASSERT(high_limit() >= low_limit(), "PointerRange/AbstractSafePointer: Integrity Error!!! Pointer destroyed");
  if ( data_limit  <  (Byte *)pointer_arg ) {
    DiagnosticPrint(KERN_EMERG "overflow: (  finish out of address space ) pointer = %p, dn = %p, up = %p, size = %lu, up-dn = %lu, data_last_byte = %p, size_parameter = %lu", pointer_arg, low_limit(), high_limit(), KCL_NAMESPACE_PERFIX SizeOf<Type>(), (Byte*)high_limit() - (Byte*)low_limit(), data_limit, length_in_bytes );
    out_error_code = kErrorOverwrite;
    return 0;
  }
  if ( pointer_arg > high_limit() ) {
    DiagnosticPrint(KERN_EMERG "overwrite: pointer = %p, dn = %p, up = %p, size = %lu, up-dn = %lu", pointer_arg, low_limit(), high_limit(), KCL_NAMESPACE_PERFIX SizeOf<Type>(), (Byte*)high_limit() - (Byte*)low_limit() );
    out_error_code = kErrorOverwrite;
    return 0;
  }
  if ( pointer_arg < low_limit() ) {
    DiagnosticPrint(KERN_EMERG "underwrite: ( begin out of range ) pointer = %p, dn = %p, up = %p, size = %lu, up-dn = %lu", pointer_arg, low_limit(), high_limit(), KCL_NAMESPACE_PERFIX SizeOf<Type>(), (Byte*)high_limit() - (Byte*)low_limit() );
    out_error_code = kErrorUnderwrite;
    return 0;
  }
  if ( data_limit  >  (Byte *)high_limit() ) {
    DiagnosticPrint(KERN_EMERG "overwrite: (  finish out of range ) pointer = %p, dn = %p, up = %p, size = %lu, up-dn = %lu, data_last_byte = %p, size_parameter = %lu", pointer_arg, low_limit(), high_limit(), KCL_NAMESPACE_PERFIX SizeOf<Type>(), (Byte*)high_limit() - (Byte*)low_limit(), data_limit, length_in_bytes );
    out_error_code = kErrorOverwrite;
    return 0;
  }
  out_error_code = kNoError;
  return 1;
}

template <typename Tn>
typename AbstractSafePointer<Tn>::ThisT&
AbstractSafePointer<Tn>::operator=(const ThisT& value) {
  new (this) AbstractSafePointer(value);
  return *this;
}

template <typename Tn>
typename AbstractSafeConstPointer<Tn>::ThisT&
AbstractSafeConstPointer<Tn>::operator=(const ThisT& value) {
  new (this) AbstractSafeConstPointer(value);
  return *this;
}


template <typename Tn>
typename SafePointer<Tn>::Type       & SafePointer<Tn>::operator*()  {
  Size error;
  if ( Verify(error) ) {
    return *get_pointer();
  } else {
    DiagnosticPrint(KERN_EMERG "error = %lu", error);
    KCL_ASSERT(0, GetErrorTextForSafePointers(error, RangeT::kOperatorDereferencing) );
    return *( KCL_CREATE_OBJECT(Type).get_pointer() );
  }
}

template <typename Tn>
typename SafePointer<Tn>::Type const & SafePointer<Tn>::operator*() const  {
  return const_cast< SafePointer<Tn> *> (this) -> operator*();
}

template <typename Tn>
typename AbstractSafePointer<Tn>::Type       * AbstractSafePointer<Tn>::operator->()  {
  Size error;
  if ( Verify(error) ) {
    return get_pointer();
  } else {
    DiagnosticPrint(KERN_EMERG "error = %lu, operator N = %d, er_lim = %lu, op_num = %lu.", error, RangeT::kOperatorAccess, (Size)PointerRangeEnums::kErrorLimit, (Size)PointerRangeEnums::kOperatorNumber);
    KCL_ASSERT(0, GetErrorTextForSafePointers(error, RangeT::kOperatorAccess) );
    return GetAddressFromPointer(KCL_CREATE_OBJECT(Type));
  }
}

template <typename Tn>
typename SafePointer<Tn>::Type       & SafePointer<Tn>::operator[](int index) {
  Size error;
  if ( Verify(error) ) {
    return get_pointer();
  } else {
    KCL_ASSERT(0, GetErrorTextForSafePointers(error, RangeT::kOperatorIndexation) );
    return KCL_CREATE_OBJECT(Type);
  }
  ;
}


template <typename Tn>
const typename SafeConstPointer<Tn>::Type & SafeConstPointer<Tn>::operator*() const {
  Size error;
  if ( Verify(error) ) {
    return *get_pointer();
  } else {
    KCL_ASSERT(0, GetErrorTextForSafePointers(error, RangeT::kOperatorDereferencing) );
    return *KCL_CREATE_OBJECT(Type);
  }
}

template <typename Tn>
const typename AbstractSafeConstPointer<Tn>::Type       * AbstractSafeConstPointer<Tn>::operator->() const {
  Size error;
  if ( Verify(error) ) {
    return get_pointer();
  } else {
    KCL_ASSERT(0, GetErrorTextForSafePointers(error, RangeT::kOperatorAccess) );
    return GetAddressFromPointer(KCL_CREATE_OBJECT(Type));
  }
}

template <typename Tn>
const typename  AbstractSafeConstPointer<Tn>::Type       * AbstractSafeConstPointer<Tn>::operator[](int index) const {
  Size error;
  if ( Verify(error) ) {
    return get_pointer();
  } else {
    KCL_ASSERT(0, GetErrorTextForSafePointers(error, RangeT::kOperatorIndexation) );
    return KCL_CREATE_OBJECT(Type);
  }
  ;
}

#endif // __cplusplus

#endif  // KC_LIBRARY_PROTECT_OFF
