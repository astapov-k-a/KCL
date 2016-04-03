#ifndef   KC_LIBRARY_PROTECT_OFF
#   error Do not include this directly!!! Use #include "KC_library.h"
#else  // KC_LIBRARY_PROTECT_OFF

typedef long RandInteger;


KCL_EXTERN RandInteger * GetKCLDummy_C(void);
KCL_EXTERN UInteger64 GetTimeInNano_C(void);

#   ifdef   __cplusplus

KCL_HEADER_DEFINITION UInteger64 GetTimeInNano() {
  return GetTimeInNano_C();
}

KCL_HEADER_DEFINITION RandInteger & GetDummy() {
  return *GetKCLDummy_C();
}

enum {
  NTAB = 32,
  NWUP = 8,
  IA = 16807,
  IM = 2147483647,
  IQ = 12773,
  IR = 2836,
  NDIV = (1+(IM-1)/NTAB)
};

/// @brief L'Ecuyer PRNG - алгоритм генерации псевдослучайных чисел имени L'Ecuyer
KCL_HEADER_DEFINITION RandInteger GetRandom(void) {
 int j;
 long k;
 static long iy=0,iv[NTAB];
 if ( GetDummy() <= 0 || !iy ) { // initialize
  if( GetDummy() < 0 ) {
    GetDummy() = - GetDummy(); //avoid negative or zero seed
  } else {
    if ( GetDummy() == 0 ) GetDummy() = 1;
    for (j = NTAB + NWUP - 1; j >= 0; j--) { //after NWUP warmups, initialize shuffle table
    k = GetDummy() / IQ;
    if (  (GetDummy() = IA * (GetDummy() - k * IQ) - IR * k) < 0 ) GetDummy() += IM;
    if (j < NTAB) iv[j] = GetDummy();
    }
  }
  iy = iv[0]; //first specimen from the table
 }
 k = GetDummy()/IQ; //regular work: generate new number
 GetDummy() = IA * ( GetDummy() - k * IQ ) - IR * k;
 if ( GetDummy() < 0 ) {
   GetDummy() += IM;
 }
 //shuffle output
 j = iy / NDIV;
 iy = iv[j];
 iv[j] = GetDummy();
 return iy;
}

#   else  //__cplusplus
KCL_EXTERN RandInteger GetRandom(void);
#   endif //__cplusplus

#endif  // KC_LIBRARY_PROTECT_OFF
