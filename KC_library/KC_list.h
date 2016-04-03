#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF


#   ifdef   __cplusplus

template <typename Tn> class ListNode;

#pragma pack(push, 1)
template <typename Tn> class PairOfWeakPtr;
#pragma pack(pop)

template <typename Tn> class ListNode : public PairOfWeakPtr<Tn> {
 public:
  typedef Tn Type;
  typedef ListNode<Tn> This;
  typedef const ListNode<Tn> ConstThis;
  typedef KCL_POINTER(This) StrongPointer;
  typedef CONST_KCL_POINTER(This) ConstStrongPointer;
  typedef KCL_WEAK_POINTER(This) WeakPointer;
  typedef CONST_KCL_WEAK_POINTER(This) ConstWeakPointer;
  typedef KCL_POINTER(Type) ObjectPointer;
  typedef CONST_KCL_POINTER(Type) ConstObjectPointer;

  class iterator;
  typedef iterator Iterator;
  typedef Type value_type;
  typedef value_type ValueType;

  ListNode()
      :  PairOfWeakPtr<Tn> (),
         object_(GetNullWeak<Type>()), autodelete_(0) {
  }
  ~ListNode() {
    DeleteObject();
  }

  KCL_FINLINE bool Initialize(const Type& to_copy);

  KCL_FINLINE bool Initialize(
      const Type& to_copy,
      WeakPointer previous_value,
      WeakPointer next_value);

  KCL_FINLINE bool FindStrong(const Type & to_find, StrongPointer & out_found, bool & out_is_locked_rule) const;
  KCL_FINLINE bool FindWeak  (const Type & to_find,   WeakPointer & out_found, bool & out_is_locked_rule) const;

  KCL_FINLINE bool SetToNoAutodelete( ObjectPointer to_set );
  KCL_FINLINE bool SetToAutodelete( ObjectPointer to_set );
  KCL_FINLINE bool SetTo( ObjectPointer to_set, bool autodelete_value = 1 );

  KCL_FINLINE bool InsertBefore(WeakPointer element);
  KCL_FINLINE bool InsertAfter(WeakPointer element);
  KCL_FINLINE bool InsertBefore(ObjectPointer reference, bool autodelete = 1);
  KCL_FINLINE bool AddBefore(const Type & element);
  KCL_FINLINE bool AddAfter(const Type & element);
  KCL_FINLINE bool AddBefore();
  KCL_FINLINE bool AddAfter();
  KCL_FINLINE void DeleteElement();
  KCL_FINLINE void DeleteThis();
  KCL_FINLINE void ExcludeThis();
  KCL_FINLINE void push_back(const Type & element, WeakPointer postend) {FindTail(postend)->AddAfter(element);}
  KCL_FINLINE void DeleteObject() {
     if ( autodelete() ) {
       KCL_DELETE_OBJECT(object());
     }
     set_autodelete(0);
     set_object(GetNullStrong<Type>());
  }
  KCL_FINLINE ConstStrongPointer GetConstPointerToThis() const {return ConstCastPointer(GetPointerToThis());}
  KCL_FINLINE StrongPointer GetPointerToThis() const {
#   ifdef    KCL_MEMORY_DEBUG_ACCESS
    return CreatePointer(this, this);
#   else  // KCL_MEMORY_DEBUG_ACCESS
    return CreatePointer(this);
#   endif // KCL_MEMORY_DEBUG_ACCESS
    return CreatePointer(this);
  }
  KCL_FINLINE bool IsHead() const {return !previous();}
  KCL_FINLINE bool IsTail() const {return !next();}
  KCL_FINLINE bool IsContainerEmpty() const {return IsHead() && IsTail() && IsElementEmpty(); }
  KCL_FINLINE bool IsOnlyOne() const {return !IsElementEmpty() && IsHead() && IsTail(); }
  KCL_FINLINE bool IsElementEmpty() const {return !Data(); }
  KCL_FINLINE bool IsNext() const {return next();}
  KCL_FINLINE bool IsPrevious() const {return previous();}


  template <class FunctorTn, typename ParametersTn, typename OutTn>
  KCL_FINLINE bool Search(ParametersTn & param, OutTn& out);

  template <class FunctorTn, typename ParamTn>
  KCL_FINLINE void DoForeach(ParamTn & param, iterator postend);

  template <class FunctorTn, typename ParametersTn, typename OutTn>
  KCL_FINLINE bool Search(ParametersTn & param, OutTn& out) const {
    return GetSelfAddress()->Search< FunctorTn, ParametersTn, OutTn > (param, out);
  }

  template <class FunctorTn, typename ParamTn>
  //KCL_FINLINE
  void DoForeach(ParamTn & param, iterator postend) const {
    bool debug_notnull = (bool)postend; /// @debug
    DiagnosticPrint("DoForeach: is postend notnull = %d", (int)((bool)postend) );
    DiagnosticPrint("DoForeach: is postend notnull = %d", (int)((bool)postend) );
    GetSelfAddress()->DoForeach<FunctorTn,ParamTn>(param, postend);
    KCL_USED(debug_notnull); /// @debug
  }

  KCL_FINLINE WeakPointer FindHead(WeakPointer prestart);
  KCL_FINLINE ConstWeakPointer FindHead(WeakPointer prestart) const { return ConstCastWeakPointer( GetSelfAddress()->FindHead(prestart) );}
  KCL_FINLINE WeakPointer FindTail(WeakPointer postend);
  KCL_FINLINE ConstWeakPointer FindTail(WeakPointer postend) const { return ConstCastWeakPointer( GetSelfAddress()->FindTail(postend) );}
  KCL_FINLINE void UnsafeSetNext    (WeakPointer value) {
    set_next     (value);
  }
  KCL_FINLINE void UnsafeSetPrevious(WeakPointer value) {
    set_previous(value);
  }
  KCL_FINLINE WeakPointer GetNext() {return next();}
  KCL_FINLINE WeakPointer GetPrevious() {return previous();}
  KCL_FINLINE ConstWeakPointer GetNext() const {return next();}
  KCL_FINLINE ConstWeakPointer GetPrevious() const {return previous();}
  KCL_FINLINE ConstWeakPointer GetConstNext() const {return ConstCastWeakPointer( next() );}
  KCL_FINLINE ConstWeakPointer GetConstPrevious() const {return ConstCastWeakPointer( previous() );}
  KCL_FINLINE ObjectPointer Access() {return object();}
  KCL_FINLINE ConstObjectPointer Data() const {return object_const();}
  KCL_FINLINE iterator GetIterator() {return iterator(this);}
  KCL_FINLINE WeakPointer      GetThisWeak() const {return StrongToWeak(GetPointerToThis());}
  KCL_FINLINE ConstWeakPointer GetConstThisWeak() const {return ConstCastWeakPointer(GetThisWeak());}
  //KCL_FINLINE
  Size size(iterator postend) const;

#      ifdef    KCL_MEMORY_DEBUG_ACCESS
  KCL_FINLINE WeakPointer      GetThis() {return GetThisWeak();}
  KCL_FINLINE ConstWeakPointer GetThis() const {return GetConstThisWeak();}
#      else  // KCL_MEMORY_DEBUG_ACCESS
  KCL_FINLINE WeakPointer      GetThis() {return this;}
  KCL_FINLINE ConstWeakPointer GetThis() const {return this;}
#      endif // KCL_MEMORY_DEBUG_ACCESS

  KCL_FINLINE void ClearAll();
  KCL_FINLINE void ClearElement();
  KCL_FINLINE static KCL_POINTER(This) Create();
  KCL_FINLINE static KCL_POINTER(This) Create(const Type& value);

 protected:
  class CalculateSizeFunctor;
  class CompareFunctor;
  KCL_FINLINE void VerifyInsertParameters(WeakPointer previous_value, WeakPointer next_value) {
    KCL_USED(previous_value);
    KCL_USED(next_value);
    KCL_ASSERT(previous_value != GetThisWeak(), "Create loop in kcl list, previous parameter error");
    KCL_ASSERT(next_value != GetThisWeak(), "Create loop in kcl list, next parameter error");
    bool prev_n_next_is_listobj = (previous_value == next_value) && (next_value) && (next_value->next() == next_value) && (next_value->previous() == next_value); //в качестве postend значения взят описатель списка List, у пустого списка в качестве "головы" и "хвоста" используются адрес его самого (хотя List не является потомком ListNode, однако, сделано так, чтобы хвост списка совпадал с полем next_ у ListNode, а поле "голова списка" с полем previous_ у ListNode
    KCL_ASSERT( prev_n_next_is_listobj || !next_value || (next_value != previous_value) , "Create loop in kcl list, next and previous are equal (parameter error)");
    KCL_ASSERT( prev_n_next_is_listobj || !next_value  || !previous_value || next_value->previous() != previous_value->next() , "Create loop in kcl list, next MUST be after previous  (parameter error)");
  }
  KCL_FINLINE void InsertThisToList_(WeakPointer previous_value, WeakPointer next_value) {
    set_next(next_value);
    set_previous(previous_value);
    if (previous_value) {
      previous_value->set_next( GetThis() );
    }
    if (next_value) {
      next_value->set_previous( GetThis() );
    }
  }

  KCL_FINLINE WeakPointer next() const     {return this->first();}
  KCL_FINLINE WeakPointer previous() const     {return this->second();}
  KCL_FINLINE void set_next(WeakPointer address) { this->first(address);}
  KCL_FINLINE void set_previous(WeakPointer address) {this->second(address);}
  KCL_FINLINE ObjectPointer object() {return object_;}
  KCL_FINLINE void set_object(ObjectPointer address) {
    object_ = address;
  }
  KCL_FINLINE void reset_object(void) {object_ = GetNullStrong<Type>();}
  KCL_FINLINE bool autodelete() const {return autodelete_;}
  KCL_FINLINE void set_autodelete(bool value) {autodelete_ = value;}

  KCL_FINLINE This       * GetSelfAddress()      const {return (      This *)this;}
  KCL_FINLINE This const * GetConstSelfAddress() const {return (const This *)this;}
  KCL_FINLINE ConstObjectPointer object_const() const {return ConstCastPointer(object_);}


 private:
  ObjectPointer object_;
  bool autodelete_;
};


///////////////////////////////////////////////////////////////////////////////
///
///  ListNode<Tn> functions
///
///////////////////////////////////////////////////////////////////////////////


template <typename Tn>
bool ListNode<Tn>::Initialize(const Type& to_copy) {
  set_object(KCL_CREATE_OBJECT(Type));
  set_autodelete(1);
  if ( object() ) {
    *object() = to_copy;
    return 1;
  } else {
    return 0;
  }
}

template <typename Tn>
bool ListNode<Tn>::Initialize(
    const Type& to_copy,
    WeakPointer previous_value,
    WeakPointer next_value) {
  VerifyInsertParameters();
  bool success = Initialize(to_copy);
  if (success) {
    InsertThisToList_(previous_value, next_value);
  }
  return success;
}


template <typename Tn>
bool ListNode<Tn>::SetTo( ObjectPointer to_set, bool autodelete_value ) {
  set_autodelete(autodelete_value);
  set_object(to_set);
  return 1;
}

template <typename Tn>
bool ListNode<Tn>::SetToNoAutodelete( ObjectPointer to_set ) {
  set_autodelete(0);
  set_object(to_set);
  return 1;
}

template <typename Tn>
void ListNode<Tn>::ClearElement( void ) {
  set_autodelete(0);
  set_object(GetNullStrong<Type>());
}

template <typename Tn>
bool ListNode<Tn>::SetToAutodelete( ObjectPointer to_set ) {
  set_autodelete(1);
  set_object(to_set);
  return 1;
}

template <typename Tn>
bool ListNode<Tn>::InsertBefore(WeakPointer element) {
  KCL_ASSERT(element, "Insert before: NULL inserting");
  element->VerifyInsertParameters(previous(), GetThis() );
  element->InsertThisToList_(previous(), GetThis() );
  return 1;
}

template <typename Tn>
KCL_FINLINE bool ListNode<Tn>::InsertBefore     (ObjectPointer reference, bool autodelete) {
  StrongPointer to_insert = KCL_CREATE_OBJECT(This);
  to_insert->SetTo(reference, autodelete);
  bool re = InsertBefore(to_insert);
  return re;
}

template <typename Tn>
bool ListNode<Tn>::InsertAfter(WeakPointer element) {
  KCL_ASSERT(element, "Insert after: NULL inserting");
  element->VerifyInsertParameters(GetThis(), next() );
  element->InsertThisToList_(GetThis(), next() );
  return 1;
}

template <typename Tn>
bool ListNode<Tn>::AddBefore(const Type & element) {
  bool re = AddBefore();
  if (re) {
    (*previous()->Access()) = element;
  }
  return re;
}

template <typename Tn>
bool ListNode<Tn>::AddAfter(const Type & element) {
  bool re = AddAfter();
  if (re) {
    (*next()->Access()) = element;
  }
return re;
}

template <typename Tn>
bool ListNode<Tn>::AddBefore() {
  WeakPointer element = KCL_CREATE_OBJECT(This) ;
  if (!element) return 0;
  ObjectPointer new_obj = KCL_CREATE_OBJECT(Type);
  set_autodelete(1);
  if (!new_obj) {
    KCL_DELETE_OBJECT(element);
    return 0;
  }
  bool re = InsertBefore(element);
  element->set_object(new_obj);
  return re;
}

template <typename Tn>
bool ListNode<Tn>::AddAfter() {
  WeakPointer element = KCL_CREATE_OBJECT(This) ;
  if (!element) return 0;
  ObjectPointer new_obj = KCL_CREATE_OBJECT(Type);
  set_autodelete(1);
  if (!new_obj) {
    KCL_DELETE_OBJECT(element);
    return 0;
  }
  bool re = InsertAfter(element);
  element->set_object(new_obj);
  return re;
}

template <typename Tn>
void ListNode<Tn>::DeleteElement() {
  if ( autodelete() ) KCL_DELETE_OBJECT(object());
  reset_object();
  set_autodelete(0);
}

template <typename Tn>
void ListNode<Tn>::DeleteThis() {
  ExcludeThis();
  KCL_DELETE_OBJECT(GetThis());
}

template <typename Tn>
void ListNode<Tn>::ExcludeThis() {
  KCL_ASSERT(this, "Dereferencing error!!!");
  WeakPointer next_value = next();
  WeakPointer prev_value = previous();
  if (next_value) {
    next_value->set_previous(prev_value);
  }
  if (prev_value) {
    prev_value->set_next(next_value);
  }
}

template <typename Tn>
typename ListNode<Tn>::WeakPointer ListNode<Tn>::FindHead(WeakPointer prestart) {
  WeakPointer re = GetThis();
  while (re->previous() != prestart) {
    re = re->previous();
  }
  return re;
}

template <typename Tn>
KCL_FINLINE KCL_POINTER(ListNode<Tn>) ListNode<Tn>::Create() {
  StrongPointer new_node = KCL_CREATE_OBJECT(This);
  KCL_ASSERT(new_node, "ListNode::Create : not enough memory");
  return new_node;
}

template <typename Tn>
KCL_FINLINE KCL_POINTER(ListNode<Tn>) ListNode<Tn>::Create(const Type& value) {
  StrongPointer new_node = Create();
  if (new_node) {
    new_node->Initialize( value );
  }
  return new_node;
}


template <typename Tn>
template <class FunctorTn, typename ParametersTn, typename OutTn>
KCL_FINLINE bool ListNode<Tn>::Search(ParametersTn & param, OutTn& out) {
  bool re = 0;
  iterator start = this;
  if ( FunctorTn::Do(start, param, out) ) {
    re = 1;
    goto finish;
  }
  for (iterator current = start->GetNext();
       current;
       current = current->GetNext() ) {
    if ( FunctorTn::Do(current, param, out) ) {
      re = 1;
      goto finish;
    }
  }
  for (iterator current = start->GetPrevious();
       current;
       current = current->GetPrevious() ) {
    if ( FunctorTn::Do(current, param, out) ) {
      re = 1;
      goto finish;
    }
  }
finish:
  return re;
}

template <typename Tn>
class ListNode<Tn>::CompareFunctor {
 public:
  KCL_FINLINE static bool Do(iterator first, Tn& second, iterator & out) {
    bool re = (*first) == second;
    if (re) out = first;
    return re;
  }
};

template <typename Tn>
KCL_FINLINE bool ListNode<Tn>::FindStrong(const Type & to_find, StrongPointer & out_found, bool & out_is_locked_rule) const {
  iterator out;
  iterator start = GetIterator();
  bool re = Search<CompareFunctor> (to_find, out);
  out_found = out->GetPointerToThis();
  return re;
}

template <typename Tn>
KCL_FINLINE bool ListNode<Tn>::FindWeak  (const Type & to_find,   WeakPointer & out_found, bool & out_is_locked_rule) const {
  iterator out;
  iterator start = GetIterator();
  bool re = Search<CompareFunctor> (to_find, out);
  out_found = out->GetThisWeak();
  return re;
}



template <typename Tn>
template <class FunctorTn, typename ParamTn>
KCL_FINLINE void ListNode<Tn>::DoForeach(ParamTn & param, iterator postend) {
  if ( IsContainerEmpty() ) return;
  iterator start = GetIterator();
  FunctorTn::Do(start, param);
  for (iterator current = start->GetNext();
       current != postend;
       current = current->GetNext() ) {
    FunctorTn::Do(current, param);
  }
  if (start == postend) return; //для случая кольцевого списка, когда postend указывает на this
  for (iterator current = start->GetPrevious();
       current != postend;
       current = current->GetPrevious() ) {
    FunctorTn::Do(current, param);
  }
}

template <typename Tn>
class ListNode<Tn>::CalculateSizeFunctor {
 public:
  KCL_FINLINE static void Do(iterator current, Size& size_counter) {
    ++size_counter;
    KCL_USED(current);
  }
};

template <typename Tn>
Size ListNode<Tn>::size(iterator postend) const {
  Size re = 0;
  DoForeach<CalculateSizeFunctor>(re,postend);
  return re;
}


template <typename Tn>
void ListNode<Tn>::ClearAll() {
  WeakPointer head = FindHead(),
              tail = FindTail(),
              old_current = GetNullWeak<This>(),
              this_val = GetThis();
  WeakPointer current = head;
  while (old_current != tail) {
    old_current = current;
    KCL_ASSERT(current , "ClearAll: integrity error!!! no next");
    current = current->GetNext();
    if (old_current != this_val) old_current->DeleteThis();
  };
  DeleteObject();
}

template <typename Tn>
typename ListNode<Tn>::WeakPointer
ListNode<Tn>::FindTail(WeakPointer postend) {
    WeakPointer re = GetThis();
    while ( re->next() != postend ) {
      re = re->next();
    }
    return re;
  }

///////////////////////////////////////////////////////////////////////////////
///
///  PairOfWeakPtr<Tn>
///
///////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
template <typename Tn> class PairOfWeakPtr {
 public:
  typedef Tn Type;
  typedef PairOfWeakPtr<Type> This;
  typedef ListNode<Type> Node;
  typedef KCL_WEAK_POINTER(Node) WeakPointer;

  KCL_FINLINE PairOfWeakPtr()
      :  first_(GetNullWeak<Node>()),
         second_(GetNullWeak<Node>()) {}

  KCL_FINLINE PairOfWeakPtr(WeakPointer value1, WeakPointer value2)
      :  first_ ( value1 ),
         second_( value2 ) {}

 protected:
  KCL_FINLINE WeakPointer first () const {return first_;}
  KCL_FINLINE WeakPointer second() const {return second_;}
  KCL_FINLINE void first(WeakPointer value) {first_ = value;}
  KCL_FINLINE void second(WeakPointer value) {second_ = value;}

 private:
  WeakPointer first_;
  WeakPointer second_;
};
#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////////
///
///  ListNode<Tn>::iterator
///
///////////////////////////////////////////////////////////////////////////////

template <typename Tn>
class ListNode<Tn>::iterator {
 public:
  friend class ListNode<Tn>;
  iterator()    :  pointer_(GetNullWeak<This>()) {}
#ifdef    KCL_MEMORY_DEBUG_ACCESS
  iterator(      This * arg)    :  pointer_(arg->GetThisWeak()) {}
  iterator(const This * arg)    :  pointer_( arg->GetThisWeak() ) {}
#endif // KCL_MEMORY_DEBUG_ACCESS
  iterator(     WeakPointer arg)    :  pointer_(arg) {}
  iterator(ConstWeakPointer arg)    :  pointer_( UnconstCastWeakPointer(arg) ) {}
  ConstWeakPointer operator->() const {return ConstCastWeakPointer(pointer());}
  WeakPointer      operator->()       {return pointer();}
  This const & operator*() const {return *pointer();}
  This       & operator*()       {return *pointer();}
  iterator operator++();
  iterator operator++(int);
  iterator operator--();
  iterator operator--(int);
  This const & operator[](Size index) const;
  This       & operator[](Size index);
  const iterator * GetConstThis() const {return this;}
  iterator * GetThis() const {return this;}
  operator bool() const {return (bool)pointer();}
  inline bool operator==(WeakPointer to_compare) {return to_compare == pointer();}
  inline bool operator!=(WeakPointer to_compare) {return to_compare != pointer();}
  inline bool operator==(iterator to_compare) {return to_compare.pointer() == pointer();}
  inline bool operator!=(iterator to_compare) {return to_compare.pointer() != pointer();}

 protected:
  WeakPointer pointer() const {return pointer_;}
  void pointer(WeakPointer arg) {pointer_ = arg;}
  void Decrease_();
  void Increase_();

 private:
  WeakPointer pointer_;
};

///////////////////////////////////////////////////////////////////////////////
///
///  ListNode<Tn>::iterator FUNCTIONS
///
///////////////////////////////////////////////////////////////////////////////


template <typename Tn>
typename ListNode<Tn>::iterator
ListNode<Tn>::iterator::operator++() {
  Increase_();
  return *this;
}

template <typename Tn>
typename ListNode<Tn>::iterator
ListNode<Tn>::iterator::operator++(int) {
  iterator re = *this;
  Increase_();
  return re;
}

template <typename Tn>
typename ListNode<Tn>::iterator
ListNode<Tn>::iterator::operator--() {
  Decrease_();
  return *this;
}

template <typename Tn>
typename ListNode<Tn>::iterator
ListNode<Tn>::iterator::operator--(int) {
  iterator re = *this;
  Decrease_();
  return re;
}

template <typename Tn>
typename ListNode<Tn>::This const &
ListNode<Tn>::iterator::operator[](Size index) const {
  return const_cast<const This&>( GetThis()->operator[](index) );
}

template <typename Tn>
typename ListNode<Tn>::This       &
ListNode<Tn>::iterator::operator[](Size index) {
  This * data = pointer();
  for (Size counter = index; counter; --counter) {
    KCL_ASSERT(pointer(), "ListNode<Tn>::iterator::operator[] index out of range");
    if (pointer()) return *this;
    data = data->GetNext();
  }
  return *data;
}

template <typename Tn>
void
ListNode<Tn>::iterator::Decrease_() {
  KCL_ASSERT(pointer(), "ListNode<Tn>::iterator::Decrease_ null pointer decrement");
  pointer(pointer()->GetPrevious());
}

template <typename Tn>
void
ListNode<Tn>::iterator::Increase_() {
  KCL_ASSERT(pointer(), "ListNode<Tn>::iterator::Increase_ null pointer increment");
  pointer(pointer()->GetNext());
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///  List<Tn>
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template <typename Tn> class List : public PairOfWeakPtr<Tn>  {
 public:
  typedef Tn Type;
  typedef List<Tn> This;
  typedef ListNode<Tn> Parent;
  typedef const List<Tn> ConstThis;
  typedef ListNode<Tn> Node;
  typedef const ListNode<Tn> ConstNode;
  typedef typename Node::StrongPointer StrongPointer;
  typedef CONST_KCL_POINTER(This) ConstStrongPointer;
  typedef typename Node::WeakPointer WeakPointer;
  typedef typename Node::ConstWeakPointer ConstWeakPointer;
  typedef typename Node::ObjectPointer ObjectPointer;
  typedef typename Node::ConstObjectPointer ConstObjectPointer;
  typedef typename Node::iterator NodeIterator;
  typedef typename Node::ValueType value_type;
  typedef typename Node::ValueType ValueType;
  class iterator;
  typedef iterator const_iterator;
  typedef iterator Iterator;
  typedef const_iterator ConstIterator;
  enum {
    kAutoDelete = 1,
    kExternalDelete = 0,
    kSetAsOwner = kAutoDelete,
    kExternalOwner = kExternalDelete
  };
  List()
      :  PairOfWeakPtr<Tn>( PostEndAsWeak(), PostEndAsWeak() ),
         number_elements_(0) {}
  ~List() {
    clear();
  }

  KCL_FINLINE bool FindStrong(
      const Type & to_find,
      WeakPointer & out_found,
      bool & out_is_locked_rule ) const {
    return Node::FindStrong(to_find, out_found, out_is_locked_rule);
  }
  KCL_FINLINE bool FindWeak(
      const Type & to_find,
      WeakPointer & out_found,
      bool & out_is_locked_rule ) const {
    return Node::FindWeak(to_find, out_found, out_is_locked_rule);
  }
  KCL_FINLINE Size size() const;


  KCL_FINLINE bool Initialize() {return 1;}

  KCL_FINLINE bool Initialize(const Type& to_copy);

  KCL_FINLINE bool Initialize(
      const Type& to_copy,
      WeakPointer previous_value,
      WeakPointer next_value);

  template <class FunctorTn, typename ParametersTn, typename OutTn>
  KCL_FINLINE bool Search(ParametersTn & param, OutTn& out);

  template <class FunctorTn, typename ParametersTn, typename OutTn>
  KCL_FINLINE bool Search(ParametersTn & param, OutTn& out, Size start_index, Size number_elements_to_iterate);

  template <class FunctorTn>
  KCL_FINLINE void DoForeach();
  template <class FunctorTn, typename ParamTn>
  KCL_FINLINE void DoForeach(ParamTn & param);

  template <class FunctorTn, typename ParametersTn, typename OutTn>
  KCL_FINLINE bool Search(ParametersTn & param, OutTn& out) const {
    return GetSelfAddress()->Search< FunctorTn, ParametersTn, OutTn > (param, out);
  }

  template <class FunctorTn, typename ParamTn>
  KCL_FINLINE void DoForeach(ParamTn & param) const {
    GetSelfAddress()->DoForeach<FunctorTn,ParamTn>(param);
  }

  KCL_FINLINE iterator begin() const {
    if ( !size() ) return end();
    return iterator(head()->GetIterator());
  }
  KCL_FINLINE iterator end() const  {
    return PostEndAsWeak()->GetIterator();
  }
  KCL_FINLINE WeakPointer FindHead() {return StrongToWeak( head() );}
  KCL_FINLINE ConstWeakPointer FindHead() const { return ConstCastWeakPointer( GetSelfAddress()->FindHead() );}
  KCL_FINLINE WeakPointer FindTail() {return tail();}
  KCL_FINLINE ConstWeakPointer FindTail() const { return ConstCastWeakPointer( GetSelfAddress()->FindTail() );}

  KCL_FINLINE KCL_POINTER(Node) CreateNode() {
    StrongPointer new_node = Node::Create();
    if (new_node) {
      tail(new_node);
      head(new_node);
      IncreaseSize();
    }
    return new_node;
  }
  KCL_FINLINE KCL_POINTER(Node) CreateNode(const Type& value) {
    StrongPointer new_node = Node::Create(value);
    return new_node;
  }

  class ClearFunctor {
   public:
    KCL_FINLINE static void Do(iterator data) {data.GetNode()->DeleteThis();}
  };
  void clear() { DoForeach<ClearFunctor>(); }
  KCL_FINLINE bool IsContainerEmpty() {return size() == 0;}

  KCL_FINLINE bool push_back (const Type& value) {
    return AddAfter(value);
  }
  KCL_FINLINE bool AddAfter(  const Type& value) {
    if (size() == 0) {
      StrongPointer new_node = CreateNode(value);
      Assign_(new_node);
      return 1;
    }
    bool re = tail()->AddAfter(value);
    if (re) {
      tail( tail()->GetNext() );
      IncreaseSize();
    }
    return re;
  }
  KCL_FINLINE Iterator insert(Iterator position, ObjectPointer value) {
    Iterator re;
    bool success = position.GetNode()->InsertBefore( value, (bool)kAutoDelete);
    if (success) {
      re = begin();
      if (re == position) {
        WeakPointer position_node = position.GetNode();
        WeakPointer new_head = position_node->GetPrevious();
        head( new_head );
        re = head()->GetIterator();
        if (tail() == position_node) {
          tail(new_head);
        }
      }
      IncreaseSize();
    }
    return re;
  }

  KCL_FINLINE Iterator erase(Iterator position) {
    Iterator re;
    if (size() == 0) return end();
    if (position.GetNode()->GetNext()) {
      re = position.GetNode()->GetNext()->GetIterator();
    }
    Delete(position.GetNode());
    return re;
  }

  KCL_FINLINE bool InsertAfter     (WeakPointer reference) {
    if (size() == 0) {
      Assign_(reference);
      return 1;
    }
    bool re = tail()->InsertAfter(reference);
    if (re) {
      tail( reference );
      IncreaseSize();
    }
    return re;
  }
  KCL_FINLINE bool AddBefore(  const Type& value) {
    bool re = head()->AddBefore(value);
    if (re) {
      head( head()->GetPrevious() );
      IncreaseSize();
    }
    return re;
  }
  KCL_FINLINE bool InsertBefore     (WeakPointer reference) {
    bool re = head()->InsertBefore(reference);
    if (re) {
      head( head->GetPrevious() );
      IncreaseSize();
    }
    return re;
  }
  KCL_FINLINE bool InsertBefore     (ObjectPointer reference, bool autodelete = 1) {
    StrongPointer to_insert = KCL_CREATE_OBJECT(Node);
    to_insert->SetTo(reference, autodelete);
    bool re = InsertBefore(to_insert);
    return re;
  }

  KCL_FINLINE void DeleteHead() {
    if ( size() != 0 ) {
      KCL_ASSERT(head(), "zero head, but size not zero");
      KCL_ASSERT(tail(), "zero tail, but size not zero");
      WeakPointer new_head = head()->GetNext();
      head()->DeleteThis();
      head( new_head->GetPointerToThis() );
      DecreaseSize();
    }
  }
  KCL_FINLINE void DeleteTail() {
    if ( size() != 0 ) {
      KCL_ASSERT(head(), "zero head, but size not zero");
      KCL_ASSERT(tail(), "zero tail, but size not zero");
      WeakPointer new_tail = tail()->GetPrevious();
      tail()->DeleteThis();
      tail( new_tail );
      DecreaseSize();
    }
  }
  KCL_FINLINE
  void Delete(WeakPointer to_delete) {
    KCL_ASSERT( size() != 0, "List::Delete(Weakpointer) : delete by pointer, but list is empty")
    KCL_ASSERT( head(), "List integrity error: null head & non-zero size");
    KCL_ASSERT( tail(), "List integrity error: null tail & non-zero size");
    if ( to_delete == head() ) {
      DeleteHead();
      return;
    }
    if ( to_delete == tail() ) {
      DeleteTail();
      return;
    }
    if ( size() < 3 ) return;
    DecreaseSize();
#   ifdef _DEBUG
    KCL_ASSERT(IsIn(to_delete), "List::Delete ERROR!!! Deleting non-list element");
#   endif
    to_delete->DeleteThis();
    //DecreaseSize();
  }

  KCL_FINLINE bool Delete(const Type & to_delete) {
    if ( size() == 0 ) return 0;
    KCL_ASSERT( head(), "List integrity error: null head & non-zero size");
    KCL_ASSERT( tail(), "List integrity error: null tail & non-zero size");
    if ( !head()->IsElementEmpty() && to_delete == head()->Data() ) {
      DeleteHead();
      return 1;
    }
    if ( !tail()->IsElementEmpty() && to_delete == tail()->Data() ) {
      DeleteTail();
      return 1;
    }
    if ( size() < 3 ) return 0;
    WeakPointer found = GetNullWeak<Node>();
    bool re = Find(to_delete, found, 1, size() - 2 );
    if (re) {
      found->DeleteThis();
      DecreaseSize();
    }
    return re;
  }
  class IsInFunctor {
   public:
    KCL_FINLINE static bool Do(
        iterator current,
        WeakPointer & to_find,
        WeakPointer & out_found ) {
      bool re = (current.GetNode()->GetThis() == to_find);
      if (re) {
        out_found = current.GetNode()->GetThis();
      }
      return re;
    }
  };
  class FindFunctor {
   public:
    KCL_FINLINE static bool Do(
        iterator current,
        Type & to_find,
        WeakPointer & out_found ) {
      bool re = (current->Data() == to_find);
      out_found = re;
      return re;
    }
  };
  class DeleteFunctor {
   public:
    KCL_FINLINE static void Do(
        iterator current) {
      current.GetNode()->DeleteThis();
    }
  };
  KCL_FINLINE bool IsIn(WeakPointer to_find) {
    WeakPointer out_found = GetNullWeak<Node>();
    return Search<IsInFunctor, WeakPointer, WeakPointer >(to_find, out_found);
  }
  KCL_FINLINE bool Find(const Type & to_find, WeakPointer & out_found) {
    return Search<IsInFunctor, Type, WeakPointer >(const_cast<Type &>(to_find), out_found);
  }

 protected:
  KCL_FINLINE WeakPointer head() const {return this->first();}
  KCL_FINLINE WeakPointer tail() const {return this->second();}
  KCL_FINLINE void head(WeakPointer value) {this->first(value);}
  KCL_FINLINE void tail(WeakPointer value) {this->second(value);}
  KCL_FINLINE void IncreaseSize() {++number_elements_;}
  KCL_FINLINE void DecreaseSize() {--number_elements_;}
  KCL_FINLINE void ZeroizeSize()  {number_elements_ = 0;}
  KCL_FINLINE void number_elements(Size value)  {number_elements_ = value;}
  KCL_FINLINE This       * GetSelfAddress()      const {return (      This *)this;}
  KCL_FINLINE This const * GetConstSelfAddress() const {return (const This *)this;}
  KCL_FINLINE WeakPointer GetThisAsNode() const { return AddressToWeak<Node>(  (Node*)((PairOfWeakPtr<Tn> *) this)  );}
  KCL_FINLINE WeakPointer PostEndAsWeak() const { return GetThisAsNode();}

  KCL_FINLINE ConstWeakPointer GetThisAsConstNode() const { return ConstCastWeakPointer( GetThisAsNode() );}
  KCL_FINLINE ConstWeakPointer PostEndAsConstWeak()      const { return GetThisAsConstNode();}

  KCL_FINLINE void Assign_(StrongPointer value) {
    head( StrongToWeak(value) );
    tail( StrongToWeak(value) );
    if (value) {
      number_elements(1);
      value->UnsafeSetNext( GetThisAsNode() );
      value->UnsafeSetPrevious( GetThisAsNode() );
    } else {
      number_elements(0);
    }    
    Node * debug_prev = &( * value->GetPrevious() ); KCL_USED(debug_prev);
    volatile Node * debug_next = &( * value->GetNext() );     KCL_USED(debug_next);
    Node * debug_this = &( * value);     KCL_USED(debug_this);
    debug_next+=1;
    KCL_USED(debug_next);
  }

 private:
  Size number_elements_;
};


////////////////////////////////////////////////////////////////////////////////
///
/// List<Tn>::iterator
///
////////////////////////////////////////////////////////////////////////////////


template <typename Tn>
class List<Tn>::iterator : public List<Tn>::NodeIterator {
 public:
  typedef typename List<Tn>::NodeIterator Parent;
  iterator()    :  Parent::iterator() {}
  iterator(const Parent & value)    :  Parent::iterator(value) {}

  Type const & operator*() const {return *( pointer()->Data() );}
  Type       & operator*()       {return *pointer()->Access();}
  ConstObjectPointer operator->() const {return pointer()->Data();}
  ObjectPointer      operator->()       {return pointer()->Access();}
  WeakPointer      GetNode()       {return pointer()->GetThis();}
  ConstWeakPointer GetNode() const {return pointer()->GetThis();}

 protected:
  using Parent::pointer;
};

template <typename Tn>
template <class FunctorTn, typename ParametersTn, typename OutTn>
KCL_FINLINE bool List<Tn>::Search(ParametersTn & param, OutTn& out) {
  bool re = 0;
  Size n = size();
  if (!n) return re;
  iterator current = begin();
  for (;n; --n) {
    if (FunctorTn::Do(current, param, out)) {
      re = 1;
      break;
    }
    ++current;
  }
  return re;
}


template <typename Tn>
template <class FunctorTn, typename ParametersTn, typename OutTn>
KCL_FINLINE bool List<Tn>::Search(ParametersTn & param, OutTn& out, Size start_index, Size number_elements_to_iterate) {
  bool re = 0;
  Size n = number_elements_to_iterate + start_index;
  KCL_ASSERT(start_index + n < size() , "List::Search indexation error");
  Size begin_it = n - start_index;
  if (!n) return re;
  iterator current = begin();
  iterator end_it = end();
  for (;n; --n) {
    KCL_ASSERT(current, "List::Search : List integrity error");
    KCL_ASSERT(current == end_it, "List::Search : List integrity error");
    if (n <= begin_it) {
      if (FunctorTn::Do(current, param, out)) {
        re = 1;
        break;
      }
    }
    ++current;
  }
  return re;
}

template <typename Tn>
template <class FunctorTn>
KCL_FINLINE void List<Tn>::DoForeach() {
  Size n = size();
  if (!n) return;
  iterator current = begin();
  for (;n; --n) {
    Iterator next = current;
    ++next;
    KCL_ASSERT(current, "List::DoForeach : List integrity error");
    KCL_ASSERT(GetAddressFromPointer(current.GetNode()) != (Node*)this, "List::DoForeach(param) : List integrity error, iterator == descriptor");
    FunctorTn::Do(current);
    current = next;
  }
}

template <typename Tn>
template <class FunctorTn, typename ParamTn>
KCL_FINLINE void List<Tn>::DoForeach(ParamTn & param) {
  Size n = size();
  if (!n) return;
  iterator current = begin();
  for (;n; --n) {
    Iterator next = current;
    ++next;
    KCL_ASSERT(current, "List::DoForeach(param) : List integrity error, iterator == null");
    KCL_ASSERT(GetAddressFromPointer(current.GetNode()) != (Node*)this, "List::DoForeach(param) : List integrity error, iterator == descriptor");
    FunctorTn::Do(current, param);
    current = next;
  }
}

template <typename Tn>
Size List<Tn>::size() const {
  return number_elements_;
}



#   endif //__cplusplus 

#endif  // KC_LIBRARY_PROTECT_OFF
