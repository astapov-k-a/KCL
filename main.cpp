#include <iostream>
#include <QtCore>
#include "KC_library/KC_library.h"

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Hello World!" << endl;
  kcl::List<int> list;
  KCL_USED(list);
  PODArrayHandle pod_array_handle;
  kcl::PODArray<int> * pod_ptr;
  CreatePODArray(int, 10, pod_array_handle, pod_ptr);
  DeletePODArray(pod_array_handle);
  kcl::SafePointer<int> sfp;
  KCL_USED(sfp)
  //kcl::SafeConstPointer<int> sfpc;
  //KCL_USED(sfpc)
  kcl::RandInteger irand = kcl::GetRandom();
  KCL_USED(irand);
  //kcl::String str= kcl::String::MakeReference("ddd");
  //printf("%s", str.c_str() );
  kcl::Vector<int> vect;
  KCL_USED(vect);
  kcl::BitField<8,8> bf;
  KCL_USED(bf);
  return 0;
}
