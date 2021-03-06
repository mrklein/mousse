// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pstream_reduce_ops.hpp"
#include "field_field_reuse_functions.hpp"
#define TEMPLATE template<template<class> class Field, class Type>
#include "field_field_functions_m.ipp"


namespace mousse {

// Global functions
template<template<class> class Field, class Type>
void component
(
  FieldField<Field, typename FieldField<Field, Type>::cmptType>& sf,
  const FieldField<Field, Type>& f,
  const direction d
)
{
  FOR_ALL(sf, i) {
    component(sf[i], f[i], d);
  }
}


template<template<class> class Field, class Type>
void T(FieldField<Field, Type>& f1, const FieldField<Field, Type>& f2)
{
  FOR_ALL(f1, i) {
    T(f1[i], f2[i]);
  }
}


template<template<class> class Field, class Type, int r>
void pow
(
  FieldField<Field, typename powProduct<Type, r>::type>& f,
  const FieldField<Field, Type>& vf
)
{
  FOR_ALL(f, i) {
    pow(f[i], vf[i]);
  }
}


template<template<class> class Field, class Type, int r>
tmp<FieldField<Field, typename powProduct<Type, r>::type> >
pow
(
  const FieldField<Field, Type>& f, typename powProduct<Type, r>::type
)
{
  typedef typename powProduct<Type, r>::type powProductType;
  tmp<FieldField<Field, powProductType> > tRes
  {
    FieldField<Field, powProductType>::NewCalculatedType(f)
  };
  pow<Type, r>(tRes(), f);
  return tRes;
}


template<template<class> class Field, class Type, int r>
tmp<FieldField<Field, typename powProduct<Type, r>::type> >
pow
(
  const tmp<FieldField<Field, Type> >& tf, typename powProduct<Type, r>::type
)
{
  typedef typename powProduct<Type, r>::type powProductType;
  tmp<FieldField<Field, powProductType> > tRes
  {
    reuseTmpFieldField<Field, powProductType, Type>::New(tf)
  };
  pow<Type, r>(tRes(), tf());
  reuseTmpFieldField<Field, powProductType, Type>::clear(tf);
  return tRes;
}


template<template<class> class Field, class Type>
void sqr
(
  FieldField<Field, typename outerProduct<Type, Type>::type>& f,
  const FieldField<Field, Type>& vf
)
{
  FOR_ALL(f, i) {
    sqr(f[i], vf[i]);
  }
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, typename outerProduct<Type, Type>::type> >
sqr(const FieldField<Field, Type>& f)
{
  typedef typename outerProduct<Type, Type>::type outerProductType;
  tmp<FieldField<Field, outerProductType> > tRes
  {
    FieldField<Field, outerProductType>::NewCalculatedType(f)
  };
  sqr(tRes(), f);
  return tRes;
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, typename outerProduct<Type, Type>::type> >
sqr(const tmp<FieldField<Field, Type> >& tf)
{
  typedef typename outerProduct<Type, Type>::type outerProductType;
  tmp<FieldField<Field, outerProductType> > tRes
  {
    reuseTmpFieldField<Field, outerProductType, Type>::New(tf)
  };
  sqr(tRes(), tf());
  reuseTmpFieldField<Field, outerProductType, Type>::clear(tf);
  return tRes;
}


template<template<class> class Field, class Type>
void magSqr(FieldField<Field, scalar>& sf, const FieldField<Field, Type>& f)
{
  FOR_ALL(sf, i) {
    magSqr(sf[i], f[i]);
  }
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, scalar> > magSqr(const FieldField<Field, Type>& f)
{
  tmp<FieldField<Field, scalar> > tRes
  {
    FieldField<Field, scalar>::NewCalculatedType(f)
  };
  magSqr(tRes(), f);
  return tRes;
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, scalar> > magSqr(const tmp<FieldField<Field, Type> >& tf)
{
  tmp<FieldField<Field, scalar> > tRes
  (
    reuseTmpFieldField<Field, scalar, Type>::New(tf)
  );
  magSqr(tRes(), tf());
  reuseTmpFieldField<Field, scalar, Type>::clear(tf);
  return tRes;
}


template<template<class> class Field, class Type>
void mag(FieldField<Field, scalar>& sf, const FieldField<Field, Type>& f)
{
  FOR_ALL(sf, i) {
    mag(sf[i], f[i]);
  }
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, scalar>> mag(const FieldField<Field, Type>& f)
{
  tmp<FieldField<Field, scalar> > tRes
  {
    FieldField<Field, scalar>::NewCalculatedType(f)
  };
  mag(tRes(), f);
  return tRes;
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, scalar>> mag(const tmp<FieldField<Field, Type> >& tf)
{
  tmp<FieldField<Field, scalar>> tRes
  {
    reuseTmpFieldField<Field, scalar, Type>::New(tf)
  };
  mag(tRes(), tf());
  reuseTmpFieldField<Field, scalar, Type>::clear(tf);
  return tRes;
}


template<template<class> class Field, class Type>
void cmptMax
(
  FieldField<Field, typename FieldField<Field, Type>::cmptType>& cf,
  const FieldField<Field, Type>& f
)
{
  FOR_ALL(cf, i) {
    cmptMax(cf[i], f[i]);
  }
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptMax
(
  const FieldField<Field, Type>& f
)
{
  typedef typename FieldField<Field, Type>::cmptType cmptType;
  tmp<FieldField<Field, cmptType>> tRes
  {
    FieldField<Field, cmptType>::NewCalculatedType(f)
  };
  cmptMax(tRes(), f);
  return tRes;
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptMax
(
  const tmp<FieldField<Field, Type> >& tf
)
{
  typedef typename FieldField<Field, Type>::cmptType cmptType;
  tmp<FieldField<Field, cmptType>> tRes
  {
    reuseTmpFieldField<Field, cmptType, Type>::New(tf)
  };
  cmptMax(tRes(), tf());
  reuseTmpFieldField<Field, cmptType, Type>::clear(tf);
  return tRes;
}


template<template<class> class Field, class Type>
void cmptMin
(
  FieldField<Field, typename FieldField<Field, Type>::cmptType>& cf,
  const FieldField<Field, Type>& f
)
{
  FOR_ALL(cf, i) {
    cmptMin(cf[i], f[i]);
  }
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptMin
(
  const FieldField<Field, Type>& f
)
{
  typedef typename FieldField<Field, Type>::cmptType cmptType;
  tmp<FieldField<Field, cmptType> > tRes
  {
    FieldField<Field, cmptType>::NewCalculatedType(f)
  };
  cmptMin(tRes(), f);
  return tRes;
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptMin
(
  const tmp<FieldField<Field, Type> >& tf
)
{
  typedef typename FieldField<Field, Type>::cmptType cmptType;
  tmp<FieldField<Field, cmptType> > tRes
  {
    reuseTmpFieldField<Field, cmptType, Type>::New(tf)
  };
  cmptMin(tRes(), tf());
  reuseTmpFieldField<Field, cmptType, Type>::clear(tf);
  return tRes;
}


template<template<class> class Field, class Type>
void cmptAv
(
  FieldField<Field, typename FieldField<Field, Type>::cmptType>& cf,
  const FieldField<Field, Type>& f
)
{
  FOR_ALL(cf, i) {
    cmptAv(cf[i], f[i]);
  }
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptAv
(
  const FieldField<Field, Type>& f
)
{
  typedef typename FieldField<Field, Type>::cmptType cmptType;
  tmp<FieldField<Field, cmptType> > tRes
  {
    FieldField<Field, cmptType>::NewCalculatedType(f)
  };
  cmptAv(tRes(), f);
  return tRes;
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType> > cmptAv
(
  const tmp<FieldField<Field, Type> >& tf
)
{
  typedef typename FieldField<Field, Type>::cmptType cmptType;
  tmp<FieldField<Field, cmptType> > tRes
  {
    reuseTmpFieldField<Field, cmptType, Type>::New(tf)
  };
  cmptAv(tRes(), tf());
  reuseTmpFieldField<Field, cmptType, Type>::clear(tf);
  return tRes;
}


template<template<class> class Field, class Type>
void cmptMag
(
  FieldField<Field, Type>& cf,
  const FieldField<Field, Type>& f
)
{
  FOR_ALL(cf, i) {
    cmptMag(cf[i], f[i]);
  }
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > cmptMag
(
  const FieldField<Field, Type>& f
)
{
  tmp<FieldField<Field, Type> > tRes
  {
    FieldField<Field, Type>::NewCalculatedType(f)
  };
  cmptMag(tRes(), f);
  return tRes;
}


template<template<class> class Field, class Type>
tmp<FieldField<Field, Type> > cmptMag
(
  const tmp<FieldField<Field, Type> >& tf
)
{
  tmp<FieldField<Field, Type> > tRes
  {
    reuseTmpFieldField<Field, Type, Type>::New(tf)
  };
  cmptMag(tRes(), tf());
  reuseTmpFieldField<Field, Type, Type>::clear(tf);
  return tRes;
}


#define TMP_UNARY_FUNCTION(returnType, func)                                  \
                                                                              \
template<template<class> class Field, class Type>                             \
returnType func(const tmp<FieldField<Field, Type> >& tf1)                     \
{                                                                             \
  returnType res = func(tf1());                                               \
  tf1.clear();                                                                \
  return res;                                                                 \
}

template<template<class> class Field, class Type>
Type max(const FieldField<Field, Type>& f)
{
  label i = 0;
  while (i < f.size() && !f[i].size())
    i++;
  if (i < f.size()) {
    Type Max(max(f[i]));
    for (label j=i+1; j<f.size(); j++) {
      if (f[j].size()) {
        Max = max(max(f[j]), Max);
      }
    }
    return Max;
  } else {
    return pTraits<Type>::min;
  }
}
TMP_UNARY_FUNCTION(Type, max);


template<template<class> class Field, class Type>
Type min(const FieldField<Field, Type>& f)
{
  label i = 0;
  while (i < f.size() && !f[i].size())
    i++;
  if (i < f.size()) {
    label i = 0;
    while (!f[i].size())
      i++;
    Type Min(min(f[i]));
    for (label j=i+1; j<f.size(); j++) {
      if (f[j].size()) {
        Min = min(min(f[j]), Min);
      }
    }
    return Min;
  } else {
    return pTraits<Type>::max;
  }
}
TMP_UNARY_FUNCTION(Type, min);


template<template<class> class Field, class Type>
Type sum(const FieldField<Field, Type>& f)
{
  if (f.size()) {
    Type Sum = pTraits<Type>::zero;
    FOR_ALL(f, i) {
      Sum += sum(f[i]);
    }
    return Sum;
  } else {
    return pTraits<Type>::zero;
  }
}
TMP_UNARY_FUNCTION(Type, sum);


template<template<class> class Field, class Type>
scalar sumMag(const FieldField<Field, Type>& f)
{
  if (f.size()) {
    scalar SumMag = 0.0;
    FOR_ALL(f, i) {
      SumMag += sumMag(f[i]);
    }
    return SumMag;
  } else {
    return 0.0;
  }
}
TMP_UNARY_FUNCTION(scalar, sumMag);


template<template<class> class Field, class Type>
Type average(const FieldField<Field, Type>& f)
{
  if (f.size()) {
    label n = 0;
    FOR_ALL(f, i) {
      n += f[i].size();
    }
    if (n == 0) {
      WARNING_IN("average(const FieldField<Field, Type>&) const")
        << "empty fieldField, returning zero" << endl;
      return pTraits<Type>::zero;
    }
    Type avrg = sum(f)/n;
    return avrg;
  } else {
    WARNING_IN("average(const FieldField<Field, Type>&) const")
      << "empty fieldField, returning zero" << endl;
    return pTraits<Type>::zero;
  }
}
TMP_UNARY_FUNCTION(Type, average);

#include "pstream_reduce_ops.hpp"

#define G_UNARY_FUNCTION(returnType, gFunc, func, rFunc)                      \
                                                                              \
template<template<class> class Field, class Type>                             \
returnType gFunc(const FieldField<Field, Type>& f)                            \
{                                                                             \
  returnType res = func(f);                                                   \
  reduce(res, rFunc##Op<Type>());                                             \
  return res;                                                                 \
}                                                                             \
TMP_UNARY_FUNCTION(returnType, gFunc)


G_UNARY_FUNCTION(Type, gMax, max, max)
G_UNARY_FUNCTION(Type, gMin, min, min)
G_UNARY_FUNCTION(Type, gSum, sum, sum)
G_UNARY_FUNCTION(scalar, gSumMag, sumMag, sum)

#undef G_UNARY_FUNCTION

template<template<class> class Field, class Type>
Type gAverage(const FieldField<Field, Type>& f)
{
  label n = 0;
  FOR_ALL(f, i) {
    n += f[i].size();
  }
  reduce(n, sumOp<label>());
  if (n > 0) {
    Type avrg = gSum(f)/n;
    return avrg;
  } else {
    WARNING_IN("gAverage(const FieldField<Field, Type>&) const")
      << "empty fieldField, returning zero" << endl;
    return pTraits<Type>::zero;
  }
}
TMP_UNARY_FUNCTION(Type, gAverage);

#undef TMP_UNARY_FUNCTION

BINARY_FUNCTION(Type, Type, Type, max)
BINARY_FUNCTION(Type, Type, Type, min)
BINARY_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_FUNCTION(Type, Type, Type, cmptDivide)
BINARY_TYPE_FUNCTION(Type, Type, Type, max)
BINARY_TYPE_FUNCTION(Type, Type, Type, min)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptDivide)

// Global operators

UNARY_OPERATOR(Type, Type, -, negate)
BINARY_OPERATOR(Type, Type, scalar, *, multiply)
BINARY_OPERATOR(Type, scalar, Type, *, multiply)
BINARY_OPERATOR(Type, Type, scalar, /, divide)
BINARY_TYPE_OPERATOR_SF(Type, scalar, Type, *, multiply)
BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, *, multiply)
BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, /, divide)


#define PRODUCT_OPERATOR(product, op, opFunc)                                 \
                                                                              \
template                                                                      \
<                                                                             \
  template<class> class Field1,                                               \
  template<class> class Field2,                                               \
  class Type1,                                                                \
  class Type2                                                                 \
>                                                                             \
void opFunc                                                                   \
(                                                                             \
  FieldField<Field1, typename product<Type1, Type2>::type>& f,                \
  const FieldField<Field1, Type1>& f1,                                        \
  const FieldField<Field2, Type2>& f2                                         \
)                                                                             \
{                                                                             \
  FOR_ALL(f, i) {                                                             \
    opFunc(f[i], f1[i], f2[i]);                                               \
  }                                                                           \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
  template<class> class Field1,                                               \
  template<class> class Field2,                                               \
  class Type1,                                                                \
  class Type2                                                                 \
>                                                                             \
tmp<FieldField<Field1, typename product<Type1, Type2>::type> >                \
operator op                                                                   \
(                                                                             \
  const FieldField<Field1, Type1>& f1,                                        \
  const FieldField<Field2, Type2>& f2                                         \
)                                                                             \
{                                                                             \
  typedef typename product<Type1, Type2>::type productType;                   \
  tmp<FieldField<Field1, productType> > tRes                                  \
  {                                                                           \
    FieldField<Field1, productType>::NewCalculatedType(f1)                    \
  };                                                                          \
  opFunc(tRes(), f1, f2);                                                     \
  return tRes;                                                                \
}                                                                             \
                                                                              \
template<template<class> class Field, class Type1, class Type2>               \
tmp<FieldField<Field, typename product<Type1, Type2>::type> >                 \
operator op                                                                   \
(                                                                             \
  const FieldField<Field, Type1>& f1,                                         \
  const tmp<FieldField<Field, Type2> >& tf2                                   \
)                                                                             \
{                                                                             \
  typedef typename product<Type1, Type2>::type productType;                   \
  tmp<FieldField<Field, productType> > tRes                                   \
  {                                                                           \
    reuseTmpFieldField<Field, productType, Type2>::New(tf2)                   \
  };                                                                          \
  opFunc(tRes(), f1, tf2());                                                  \
  reuseTmpFieldField<Field, productType, Type2>::clear(tf2);                  \
  return tRes;                                                                \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
  template<class> class Field1,                                               \
  template<class> class Field2,                                               \
  class Type1,                                                                \
  class Type2                                                                 \
>                                                                             \
tmp<FieldField<Field, typename product<Type1, Type2>::type> >                 \
operator op                                                                   \
(                                                                             \
  const FieldField<Field1, Type1>& f1,                                        \
  const tmp<FieldField<Field2, Type2> >& tf2                                  \
)                                                                             \
{                                                                             \
  typedef typename product<Type1, Type2>::type productType;                   \
  tmp<FieldField<Field1, productType> > tRes                                  \
  {                                                                           \
    FieldField<Field1, productType>::NewCalculatedType(f1)                    \
  };                                                                          \
  opFunc(tRes(), f1, tf2());                                                  \
  tf2.clear();                                                                \
  return tRes;                                                                \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
  template<class> class Field1,                                               \
  template<class> class Field2,                                               \
  class Type1,                                                                \
  class Type2                                                                 \
>                                                                             \
tmp<FieldField<Field1, typename product<Type1, Type2>::type> >                \
operator op                                                                   \
(                                                                             \
  const tmp<FieldField<Field1, Type1> >& tf1,                                 \
  const FieldField<Field2, Type2>& f2                                         \
)                                                                             \
{                                                                             \
  typedef typename product<Type1, Type2>::type productType;                   \
  tmp<FieldField<Field1, productType> > tRes                                  \
  {                                                                           \
    reuseTmpFieldField<Field1, productType, Type1>::New(tf1)                  \
  };                                                                          \
  opFunc(tRes(), tf1(), f2);                                                  \
  reuseTmpFieldField<Field1, productType, Type1>::clear(tf1);                 \
  return tRes;                                                                \
}                                                                             \
                                                                              \
template                                                                      \
<                                                                             \
  template<class> class Field1,                                               \
  template<class> class Field2,                                               \
  class Type1,                                                                \
  class Type2                                                                 \
>                                                                             \
tmp<FieldField<Field1, typename product<Type1, Type2>::type> >                \
operator op                                                                   \
(                                                                             \
  const tmp<FieldField<Field1, Type1> >& tf1,                                 \
  const tmp<FieldField<Field2, Type2> >& tf2                                  \
)                                                                             \
{                                                                             \
  typedef typename product<Type1, Type2>::type productType;                   \
  tmp<FieldField<Field1, productType> > tRes                                  \
  {                                                                           \
    reuseTmpTmpFieldField<Field1, productType, Type1, Type1, Type2>::New      \
    (tf1, tf2)                                                                \
  };                                                                          \
  opFunc(tRes(), tf1(), tf2());                                               \
  reuseTmpTmpFieldField<Field1, productType, Type1, Type1, Type2>::clear      \
  (tf1, tf2);                                                                 \
  return tRes;                                                                \
}                                                                             \
                                                                              \
template                                                                      \
<template<class> class Field, class Type, class Form, class Cmpt, int nCmpt>  \
void opFunc                                                                   \
(                                                                             \
  FieldField<Field, typename product<Type, Form>::type>& f,                   \
  const FieldField<Field, Type>& f1,                                          \
  const VectorSpace<Form,Cmpt,nCmpt>& vs                                      \
)                                                                             \
{                                                                             \
  FOR_ALL(f, i)                                                               \
  {                                                                           \
    opFunc(f[i], f1[i], vs);                                                  \
  }                                                                           \
}                                                                             \
                                                                              \
template                                                                      \
<template<class> class Field, class Type, class Form, class Cmpt, int nCmpt>  \
tmp<FieldField<Field, typename product<Type, Form>::type> >                   \
operator op                                                                   \
(                                                                             \
  const FieldField<Field, Type>& f1,                                          \
  const VectorSpace<Form,Cmpt,nCmpt>& vs                                      \
)                                                                             \
{                                                                             \
  typedef typename product<Type, Form>::type productType;                     \
  tmp<FieldField<Field, productType> > tRes                                   \
  {                                                                           \
    FieldField<Field, productType>::NewCalculatedType(f1)                     \
  };                                                                          \
  opFunc(tRes(), f1, static_cast<const Form&>(vs));                           \
  return tRes;                                                                \
}                                                                             \
                                                                              \
template                                                                      \
<template<class> class Field, class Type, class Form, class Cmpt, int nCmpt>  \
tmp<FieldField<Field, typename product<Type, Form>::type> >                   \
operator op                                                                   \
(                                                                             \
  const tmp<FieldField<Field, Type> >& tf1,                                   \
  const VectorSpace<Form,Cmpt,nCmpt>& vs                                      \
)                                                                             \
{                                                                             \
  typedef typename product<Type, Form>::type productType;                     \
  tmp<FieldField<Field, productType> > tRes                                   \
  {                                                                           \
    reuseTmpFieldField<Field, productType, Type>::New(tf1)                    \
  };                                                                          \
  opFunc(tRes(), tf1(), static_cast<const Form&>(vs));                        \
  reuseTmpFieldField<Field, productType, Type>::clear(tf1);                   \
  return tRes;                                                                \
}                                                                             \
                                                                              \
template                                                                      \
<template<class> class Field, class Form, class Cmpt, int nCmpt, class Type>  \
void opFunc                                                                   \
(                                                                             \
  FieldField<Field, typename product<Form, Type>::type>& f,                   \
  const VectorSpace<Form,Cmpt,nCmpt>& vs,                                     \
  const FieldField<Field, Type>& f1                                           \
)                                                                             \
{                                                                             \
  FOR_ALL(f, i) {                                                             \
    opFunc(f[i], vs, f1[i]);                                                  \
  }                                                                           \
}                                                                             \
                                                                              \
template                                                                      \
<template<class> class Field, class Form, class Cmpt, int nCmpt, class Type>  \
tmp<FieldField<Field, typename product<Form, Type>::type> >                   \
operator op                                                                   \
(                                                                             \
  const VectorSpace<Form,Cmpt,nCmpt>& vs,                                     \
  const FieldField<Field, Type>& f1                                           \
)                                                                             \
{                                                                             \
  typedef typename product<Form, Type>::type productType;                     \
  tmp<FieldField<Field, productType> > tRes                                   \
  {                                                                           \
    FieldField<Field, productType>::NewCalculatedType(f1)                     \
  };                                                                          \
  opFunc(tRes(), static_cast<const Form&>(vs), f1);                           \
  return tRes;                                                                \
}                                                                             \
                                                                              \
template                                                                      \
<template<class> class Field, class Form, class Cmpt, int nCmpt, class Type>  \
tmp<FieldField<Field, typename product<Form, Type>::type> >                   \
operator op                                                                   \
(                                                                             \
  const VectorSpace<Form,Cmpt,nCmpt>& vs,                                     \
  const tmp<FieldField<Field, Type> >& tf1                                    \
)                                                                             \
{                                                                             \
  typedef typename product<Form, Type>::type productType;                     \
  tmp<FieldField<Field, productType> > tRes                                   \
  {                                                                           \
    reuseTmpFieldField<Field, productType, Type>::New(tf1)                    \
  };                                                                          \
  opFunc(tRes(), static_cast<const Form&>(vs), tf1());                        \
  reuseTmpFieldField<Field, productType, Type>::clear(tf1);                   \
  return tRes;                                                                \
}

PRODUCT_OPERATOR(typeOfSum, +, add)
PRODUCT_OPERATOR(typeOfSum, -, subtract)
PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)
#undef PRODUCT_OPERATOR

}  // namespace mousse

#include "undef_field_functions_m.inc"
