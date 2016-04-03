#ifndef   POD_ARRAY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h" or other
#else  // POD_ARRAY_PROTECT_OFF

#   undef POD_ARRAY_PROTECT_OFF


#   ifdef  __cplusplus
#      pragma pack(push,1)

template <typename Tn> class PODArray {
 public:
  typedef Tn    Element;
  typedef PODArray<Element> This;
  typedef       Element * iterator;
  typedef const Element * const_iterator;
  Size size() const {return size_;}
  void SetSize(Size value) {size_ = value;}
  bool empty() const {return size() == 0;}
  iterator       GetReference(Size index)       {return data() + index;}
  const_iterator GetReference(Size index) const {return data() + index;}
  Element const & operator[](Size index) const {return data()[index];}
  Element       & operator[](Size index)       {return data()[index];}
  const_iterator begin() const {return data();}
  iterator       begin()       {return data();}
  const_iterator end() const {return GetReference( size() );}
  iterator       end()       {return GetReference( size() );}
  Size GetSizeof() {return GetDataOffset() + size()*sizeof(Tn);}

  static This * ReinterpretAsPODArray(void * what) {
    return static_cast<This * >(what);
  }

 protected:
  Element const * data() const {return data_;}
  Element       * data()       {return data_;}

 private:
  static inline Size GetDataOffset() {This * reference = (This *)NULL; return (Byte*)reference->data_ - (Byte*)reference;}
  PODArray()  :    size_(0) {}
  PODArray(const This &);
  This & operator=(const This &);

  Size size_;
  Element data_[1];
};

#      pragma pack(pop)
#   endif //__cplusplus

#endif  // POD_ARRAY_PROTECT_OFF
