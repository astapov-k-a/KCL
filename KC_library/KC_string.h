#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF


#   ifdef  __cplusplus

Size MemoryLength(const void * pointer);

///@brief null-terminated string
///  @warning класс должен быть легким, так как используется для вывода ошибок. Никаких сложных указателей!!!
class String {
 public:
  enum {kTerminator = 0, kTerminatorSize = 1};
  typedef Char CharType;
  typedef String This;
  String()    :  data_(NULL), size_(0), is_owner_(0) {}
  CharType * c_str() {return data();}
  void operator+=(const CharType * to_add) {Append_(  to_add, MemoryLength( reinterpret_cast<const void *>(to_add) )  );}
  Size length() const {return size();}
  static This MakeReference(const CharType *string);

 protected:
  void Append_(const CharType * to_add, Size number_chars_to_add);
  String(const This &);
  This & operator=(const This &);
  bool IsNull() {return !data_;}
  void Resize_(Size new_size);
  static CharType * MakeData_(Size new_size);
  void MakeDataForThis_(Size new_size);
  static void DeleteData_(CharType * pointer);
  void DeleteDataForThis_();

  CharType * data() const {return data_;}
  void data(CharType *value) {data_ = value;}
  Size size() const {return size_;}
  void size(Size value) {size_ = value;}
  bool is_owner() const {return is_owner_;}
  void is_owner(bool value) {is_owner_ = value;}

 private:
  CharType * data_;
  Size size_;
  bool is_owner_;
};


#   endif //__cplusplus

#endif  // KC_LIBRARY_PROTECT_OFF
