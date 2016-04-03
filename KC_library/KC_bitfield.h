#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF

#   ifdef   __cplusplus

#pragma pack(push,1)
template <typename IntegerTn = UInteger64>
class BaseFlags {
 public:
  typedef IntegerTn Integer;
  typedef BaseFlags<Integer> This;
  enum {
    kByteSize = sizeof(Integer),
    kBitSize = kByteSize<<3
  };

  explicit BaseFlags(Integer value)    :  int_(value) {}
  explicit BaseFlags()    :  int_(0) {}

  KCL_FINLINE Integer Value() {return data();}

  KCL_FINLINE void Set(Size bit_to_set) {
    KCL_ASSERT(bit_to_set < kBitSize, "BaseFlags::Set(Size) : Indexation error");
    Integer mask = 1<<bit_to_set;
    get_data() |= mask;
  }
  KCL_FINLINE void SetTo(Size bit_to_set, bool value) {
    KCL_ASSERT(bit_to_set < kBitSize, "BaseFlags::Set(Size, bool) : Indexation error");
    Integer mask = 1<<bit_to_set;
    Integer value_mask = value << bit_to_set;
    get_data() = data() & (~mask);
    get_data() |= value_mask;
  }
  KCL_FINLINE void Reset(Size bit_to_reset) {
    KCL_ASSERT(bit_to_reset < kBitSize, "BaseFlags::Reset : Indexation error");
    Integer mask = 1<<bit_to_reset;
    get_data() &= ~mask;
  }
  KCL_FINLINE bool Get(Size bit_to_get) const {
    KCL_ASSERT(bit_to_get < kBitSize, "BaseFlags::Get : Indexation error");
    Integer mask = 1<<bit_to_get;
    Integer re = data() & mask;
    return re != 0;
  }
  KCL_FINLINE void Invert(Size bit_to_invert) {
    KCL_ASSERT(bit_to_invert < kBitSize, "BaseFlags::Invert : Indexation error");
    Integer mask = 1<<bit_to_invert;
    Integer val = data() & (~mask);
    get_data() &= ~mask;
    get_data() |= ~val;
  }
  KCL_FINLINE void Clear() {get_data() = 0;}
  static This & Reinterpret(Integer & value) {return reinterpret_cast<This &>(&value);}

 protected:
  KCL_FINLINE Integer data() const {return int_;}
  KCL_FINLINE void  data(Integer value) {int_ = value;}
  KCL_FINLINE Integer & get_data() {return int_;}

 private:
  Integer int_;
};
typedef BaseFlags<UInteger64> Flags;
#pragma pack(pop)

#pragma pack(push,1)
template <Size DimensionX = 8, Size DimensionY = 1>
class BitField {
 public:
  typedef UInteger64 IntegerOfBase;
  typedef BaseFlags<IntegerOfBase> Base;
  typedef BitField<DimensionX, DimensionY> This;
  enum {
    kDimensionX = DimensionX,
    kDimensionY = DimensionY,
    kBitLength = kDimensionX * kDimensionY,
    kByteLength = KCL_GET_INFINUM_SIZE( kBitLength, sizeof(Byte) ),
    kSizeofBase = sizeof(IntegerOfBase) ,
    kSize = KCL_GET_INFINUM_SIZE(kByteLength, kSizeofBase),
    kNumberBits = kByteLength * sizeof(Byte),
    kSizeofBaseBitSize = kSizeofBase * sizeof(Byte)
  };

  KCL_FINLINE void Set(Size bit_to_set) {
    KCL_ASSERT(bit_to_set < kNumberBits, "BitField::Set(Size) : Indexation error");
    Size index = bit_to_set / kSizeofBaseBitSize;
    Size bit_number = bit_to_set % kSizeofBaseBitSize;
    get_data()[index].Set(bit_number);
  }
  KCL_FINLINE void SetTo(Size bit_to_set, bool value) {
    KCL_ASSERT(bit_to_set < kNumberBits, "BitField::Set(Size, bool) : Indexation error");
    Size index = bit_to_set / kSizeofBaseBitSize;
    Size bit_number = bit_to_set % kSizeofBaseBitSize;
    get_data()[index].SetTo(bit_number, value);
  }
  KCL_FINLINE void Reset(Size bit_to_reset) {
    KCL_ASSERT(bit_to_reset < kNumberBits, "BitField::Reset : Indexation error");
    Size index = bit_to_reset / kSizeofBaseBitSize;
    Size bit_number = bit_to_reset % kSizeofBaseBitSize;
    get_data()[index].Reset(bit_number);
  }
  KCL_FINLINE bool Get(Size bit_to_get) const {
    KCL_ASSERT(bit_to_get < kNumberBits, "BitField::Get : Indexation error");
    Size index = bit_to_get / kSizeofBaseBitSize;
    Size bit_number = bit_to_get % kSizeofBaseBitSize;
    return data()[index].Get(bit_number);
  }
  KCL_FINLINE void Invert(Size bit_to_invert) {
    KCL_ASSERT(bit_to_invert < kNumberBits, "BitField::Invert : Indexation error");
    Size index = bit_to_invert / kSizeofBaseBitSize;
    Size bit_number = bit_to_invert % kSizeofBaseBitSize;
    get_data()[index].Invert(bit_number);
  }

  KCL_FINLINE void Set(Size x, Size y)               { return Set   (x * kDimensionY + y);        }
  KCL_FINLINE void SetTo(Size x, Size y, bool value) { return SetTo (x * kDimensionY + y, value); }
  KCL_FINLINE void Reset(Size x, Size y)             { return Reset (x * kDimensionY + y);        }
  KCL_FINLINE bool Get(Size x, Size y) const         { return Get   (x * kDimensionY + y);        }
  KCL_FINLINE void Invert(Size x, Size y)            { return Invert(x * kDimensionY + y);        }

  KCL_FINLINE void Clear() {
    Size counter = 0 - kSize;
    Base * base = end_pointer();
    for (; counter; ++counter) {
      base[counter].Clear();
    }
  }

 protected:
  KCL_FINLINE Base       * get_data()       {return data_;}
  KCL_FINLINE Base const *     data() const {return data_;}
  KCL_FINLINE Base * end_pointer() {return get_data() + kSize;}

 private:
  Base data_[kSize];
};
#pragma pack(pop)

//extern BitField<16>  debug_bit_field;



#   endif //__cplusplus

#endif  // KC_LIBRARY_PROTECT_OFF
