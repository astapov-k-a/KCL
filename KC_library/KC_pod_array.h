#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF

#   ifdef  __cplusplus
#      define POD_ARRAY_PROTECT_OFF
#      include "KC_pod_array_internal.h"

#define CreatePODArray(TypeOfElement, SizeOfArray, OutHandle, OutPointer) \
    KCL_NAMESPACE_PERFIX CreatePODArrayInternal<TypeOfElement>( \
        SizeOfArray, \
        OutHandle,\
        OutPointer);
template <typename TypeOfElement> KCL_INLINE
void CreatePODArrayInternal(
    Size number_elements_of_array,
    KCL_ARRAY_POINTER(Byte) & out_handle,
    PODArray<TypeOfElement> *& out_pointer) {
  out_handle = KCL_CREATE_ARRAY(Byte, sizeof(Size)+ sizeof(TypeOfElement) * number_elements_of_array);
  if (out_handle) {
    out_pointer = reinterpret_cast< KCL_NAMESPACE_PERFIX PODArray<TypeOfElement> * >( out_handle.GetMemoryBlock () );
    out_pointer -> SetSize(number_elements_of_array);
  } else {
    out_pointer = 0;
  }
}


#define DeletePODArray(Handle) { KCL_DELETE_ARRAY(Handle);}
#define PODArrayHandle KCL_ARRAY_POINTER(Byte)

#   endif //__cplusplus

#endif  // KC_LIBRARY_PROTECT_OFF
