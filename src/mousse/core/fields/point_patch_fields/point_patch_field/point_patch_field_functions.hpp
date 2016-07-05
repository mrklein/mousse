// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse {

template<class Type>
inline void component
(
  pointPatchField<typename Field<Type>::cmptType>&,
  const pointPatchField<Type>&,
  const direction
)
{}


template<class Type>
inline void T
(
  pointPatchField<Type>&,
  const pointPatchField<Type>&
)
{}


template<class Type, int r>
inline void pow
(
  Field<typename powProduct<Type, r>::type>&,
  const pointPatchField<Type>&
)
{}


template<class Type>
inline void sqr
(
  Field<typename outerProduct<Type, Type>::type>&,
  const pointPatchField<Type>&
)
{}


template<class Type>
inline void magSqr
(
  pointPatchField<scalar>&,
  const pointPatchField<Type>&
)
{}


template<class Type>
inline void mag
(
  pointPatchField<scalar>&,
  const pointPatchField<Type>&
)
{}


template<class Type>
inline void cmptAv
(
  pointPatchField<typename Field<Type>::cmptType>&,
  const pointPatchField<Type>&
)
{}


template<class Type>
inline void cmptMag
(
  pointPatchField<Type>&,
  const pointPatchField<Type>&
)
{}


#define BINARY_FUNCTION(func)                                                 \
                                                                              \
template<class Type>                                                          \
inline void func                                                              \
(                                                                             \
  pointPatchField<Type>&,                                                     \
  const pointPatchField<Type>&,                                               \
  const pointPatchField<Type>&                                                \
)                                                                             \
{}                                                                            \
                                                                              \
template<class Type>                                                          \
inline void func                                                              \
(                                                                             \
  pointPatchField<Type>&,                                                     \
  const pointPatchField<Type>&,                                               \
  const Type&                                                                 \
)                                                                             \
{}


BINARY_FUNCTION(max)
BINARY_FUNCTION(min)
BINARY_FUNCTION(cmptMultiply)
BINARY_FUNCTION(cmptDivide)


// Global operators
#define UNARY_OPERATOR(op, opFunc)                                            \
                                                                              \
template<class Type>                                                          \
inline void opFunc                                                            \
(                                                                             \
  pointPatchField<Type>&,                                                     \
  const pointPatchField<Type>&                                                \
)                                                                             \
{}

UNARY_OPERATOR(-, negate)


#define BINARY_OPERATOR(Type1, Type2, op, opFunc)                             \
                                                                              \
template<class Type>                                                          \
inline void opFunc                                                            \
(                                                                             \
  pointPatchField<Type>&,                                                     \
  const pointPatchField<Type1>&,                                              \
  const pointPatchField<Type2>&                                               \
)                                                                             \
{}

BINARY_OPERATOR(scalar, Type, *, multiply)
BINARY_OPERATOR(Type, scalar, *, multiply)
BINARY_OPERATOR(Type, scalar, /, divide)


#define BINARY_TYPE_OPERATOR_SF(TYPE, op, opFunc)                             \
                                                                              \
template<class Type>                                                          \
inline void opFunc                                                            \
(                                                                             \
  pointPatchField<Type>&,                                                     \
  const TYPE&,                                                                \
  const pointPatchField<Type>&                                                \
)                                                                             \
{}


#define BINARY_TYPE_OPERATOR_FS(TYPE, op, opFunc)                             \
                                                                              \
template<class Type>                                                          \
inline void opFunc                                                            \
(                                                                             \
  pointPatchField<Type>&,                                                     \
  const pointPatchField<Type>&,                                               \
  const TYPE&                                                                 \
)                                                                             \
{}


BINARY_TYPE_OPERATOR_SF(scalar, *, multiply)
BINARY_TYPE_OPERATOR_FS(scalar, *, multiply)
BINARY_TYPE_OPERATOR_FS(scalar, /, divide)


#define PRODUCT_OPERATOR(product, op, opFunc)                                 \
                                                                              \
template                                                                      \
<                                                                             \
  class Type1,                                                                \
  class Type2                                                                 \
>                                                                             \
inline void opFunc                                                            \
(                                                                             \
  pointPatchField                                                             \
  <typename product<Type1, Type2>::type>&,                                    \
  const pointPatchField<Type1>&,                                              \
  const pointPatchField<Type2>&                                               \
)                                                                             \
{}                                                                            \
                                                                              \
template                                                                      \
<                                                                             \
  class Type,                                                                 \
  class Form,                                                                 \
  class Cmpt,                                                                 \
  int nCmpt                                                                   \
>                                                                             \
inline void opFunc                                                            \
(                                                                             \
  pointPatchField                                                             \
  <typename product<Type, Form>::type>&,                                      \
  const pointPatchField<Type>&,                                               \
  const VectorSpace<Form,Cmpt,nCmpt>&                                         \
)                                                                             \
{}                                                                            \
                                                                              \
template                                                                      \
<                                                                             \
  class Form,                                                                 \
  class Cmpt,                                                                 \
  int nCmpt,                                                                  \
  class Type                                                                  \
>                                                                             \
inline void opFunc                                                            \
(                                                                             \
  pointPatchField                                                             \
  <typename product<Form, Type>::type>&,                                      \
  const VectorSpace<Form,Cmpt,nCmpt>&,                                        \
  const pointPatchField<Type>&                                                \
)                                                                             \
{}


PRODUCT_OPERATOR(typeOfSum, +, add)
PRODUCT_OPERATOR(typeOfSum, -, subtract)
PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)

#undef PRODUCT_OPERATOR


inline void hdual
(
  pointPatchField<vector>&,
  const pointPatchField<tensor>&
)
{}


inline void hdual
(
  pointPatchField<tensor>&,
  const pointPatchField<vector>&
)
{}


inline void diag
(
  pointPatchField<vector>&,
  const pointPatchField<tensor>&
)
{}


inline void tr
(
  pointPatchField<scalar>&,
  const pointPatchField<tensor>&
)
{}


inline void dev
(
  pointPatchField<tensor>&,
  const pointPatchField<tensor>&
)
{}


inline void dev2
(
  pointPatchField<tensor>&,
  const pointPatchField<tensor>&
)
{}


inline void det
(
  pointPatchField<scalar>&,
  const pointPatchField<tensor>&
)
{}


inline void inv
(
  pointPatchField<tensor>&,
  const pointPatchField<tensor>&
)
{}


inline void symm
(
  pointPatchField<tensor>&,
  const pointPatchField<tensor>&
)
{}


inline void twoSymm
(
  pointPatchField<tensor>&,
  const pointPatchField<tensor>&
)
{}


inline void skew
(
  pointPatchField<tensor>&,
  const pointPatchField<tensor>&
)
{}


inline void eigenValues
(
  pointPatchField<vector>&,
  const pointPatchField<tensor>&
)
{}


inline void eigenVectors
(
  pointPatchField<tensor>&,
  const pointPatchField<tensor>&
)
{}

}  // namespace mousse

#include "undef_field_functions_m.inc"

