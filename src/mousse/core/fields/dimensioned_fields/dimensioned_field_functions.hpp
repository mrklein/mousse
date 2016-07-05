// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimensioned_scalar_field.hpp"
#define TEMPLATE template<class Type, class GeoMesh>
#include "dimensioned_field_functions_m.inc"


namespace mousse {

// Global functions 
template<class Type, class GeoMesh, int r>
tmp<DimensionedField<typename powProduct<Type, r>::type, GeoMesh> >
pow
(
  const DimensionedField<Type, GeoMesh>& df,
  typename powProduct<Type, r>::type
);

template<class Type, class GeoMesh, int r>
tmp<DimensionedField<typename powProduct<Type, r>::type, GeoMesh> >
pow
(
  const tmp<DimensionedField<Type, GeoMesh> >& tdf,
  typename powProduct<Type, r>::type
);

template<class Type, class GeoMesh>
tmp<DimensionedField<typename outerProduct<Type, Type>::type, GeoMesh> >
sqr(const DimensionedField<Type, GeoMesh>& df);
template<class Type, class GeoMesh>
tmp<DimensionedField<typename outerProduct<Type, Type>::type, GeoMesh> >
sqr(const tmp<DimensionedField<Type, GeoMesh> >& tdf);
template<class Type, class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > magSqr
(
  const DimensionedField<Type, GeoMesh>& df
);

template<class Type, class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > magSqr
(
  const tmp<DimensionedField<Type, GeoMesh> >& tdf
);
template<class Type, class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > mag
(
  const DimensionedField<Type, GeoMesh>& df
);

template<class Type, class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > mag
(
  const tmp<DimensionedField<Type, GeoMesh> >& tdf
);
template<class Type, class GeoMesh>
tmp
<
  DimensionedField
    <typename DimensionedField<Type, GeoMesh>::cmptType, GeoMesh>
>
cmptAv(const DimensionedField<Type, GeoMesh>& df);
template<class Type, class GeoMesh>
tmp
<
  DimensionedField
    <typename DimensionedField<Type, GeoMesh>::cmptType, GeoMesh>
>
cmptAv(const tmp<DimensionedField<Type, GeoMesh> >& tdf);

#define UNARY_REDUCTION_FUNCTION(returnType, func, dfunc)                     \
                                                                              \
template<class Type, class GeoMesh>                                           \
dimensioned<returnType> func                                                  \
(                                                                             \
  const DimensionedField<Type, GeoMesh>& df                                   \
);                                                                            \
template<class Type, class GeoMesh>                                           \
dimensioned<returnType> func                                                  \
(                                                                             \
  const tmp<DimensionedField<Type, GeoMesh> >& tdf1                           \
);

UNARY_REDUCTION_FUNCTION(Type, max, gMax)
UNARY_REDUCTION_FUNCTION(Type, min, gMin)
UNARY_REDUCTION_FUNCTION(Type, sum, gSum)
UNARY_REDUCTION_FUNCTION(scalar, sumMag, gSumMag)
UNARY_REDUCTION_FUNCTION(Type, average, gAverage)

#undef UNARY_REDUCTION_FUNCTION

BINARY_FUNCTION(Type, Type, Type, max)
BINARY_FUNCTION(Type, Type, Type, min)
BINARY_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_FUNCTION(Type, Type, Type, cmptDivide)
BINARY_TYPE_FUNCTION(Type, Type, Type, max)
BINARY_TYPE_FUNCTION(Type, Type, Type, min)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptDivide)
// Global operators 
UNARY_OPERATOR(Type, Type, -, negate, transform)
BINARY_OPERATOR(Type, Type, scalar, *, '*', multiply)
BINARY_OPERATOR(Type, scalar, Type, *, '*', multiply)
BINARY_OPERATOR(Type, Type, scalar, /, '|', divide)
BINARY_TYPE_OPERATOR_SF(Type, scalar, Type, *, '*', multiply)
BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, *, '*', multiply)
BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, /, '|', divide)

#define PRODUCT_OPERATOR(product, op, opFunc)                                 \
                                                                              \
template<class Type1, class Type2, class GeoMesh>                             \
tmp<DimensionedField<typename product<Type1, Type2>::type, GeoMesh> >         \
operator op                                                                   \
(                                                                             \
  const DimensionedField<Type1, GeoMesh>& df1,                                \
  const DimensionedField<Type2, GeoMesh>& df2                                 \
);                                                                            \
                                                                              \
template<class Type1, class Type2, class GeoMesh>                             \
tmp<DimensionedField<typename product<Type1, Type2>::type, GeoMesh> >         \
operator op                                                                   \
(                                                                             \
  const DimensionedField<Type1, GeoMesh>& df1,                                \
  const tmp<DimensionedField<Type2, GeoMesh> >& tdf2                          \
);                                                                            \
                                                                              \
template<class Type1, class Type2, class GeoMesh>                             \
tmp<DimensionedField<typename product<Type1, Type2>::type, GeoMesh> >         \
operator op                                                                   \
(                                                                             \
  const tmp<DimensionedField<Type1, GeoMesh> >& tdf1,                         \
  const DimensionedField<Type2, GeoMesh>& df2                                 \
);                                                                            \
                                                                              \
template<class Type1, class Type2, class GeoMesh>                             \
tmp<DimensionedField<typename product<Type1, Type2>::type, GeoMesh> >         \
operator op                                                                   \
(                                                                             \
  const tmp<DimensionedField<Type1, GeoMesh> >& tdf1,                         \
  const tmp<DimensionedField<Type2, GeoMesh> >& tdf2                          \
);                                                                            \
                                                                              \
template<class Form, class Type, class GeoMesh>                               \
tmp<DimensionedField<typename product<Type, Form>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
  const DimensionedField<Type, GeoMesh>& df1,                                 \
  const dimensioned<Form>& dvs                                                \
);                                                                            \
                                                                              \
template<class Form, class Cmpt, int nCmpt, class Type, class GeoMesh>        \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
  const DimensionedField<Type, GeoMesh>& df1,                                 \
  const VectorSpace<Form,Cmpt,nCmpt>& vs                                      \
);                                                                            \
                                                                              \
template<class Form, class Type, class GeoMesh>                               \
tmp<DimensionedField<typename product<Type, Form>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
  const tmp<DimensionedField<Type, GeoMesh> >& tdf1,                          \
  const dimensioned<Form>& dvs                                                \
);                                                                            \
                                                                              \
template<class Form, class Cmpt, int nCmpt, class Type, class GeoMesh>        \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
  const tmp<DimensionedField<Type, GeoMesh> >& tdf1,                          \
  const VectorSpace<Form,Cmpt,nCmpt>& vs                                      \
);                                                                            \
                                                                              \
template<class Form, class Type, class GeoMesh>                               \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
  const dimensioned<Form>& dvs,                                               \
  const DimensionedField<Type, GeoMesh>& df1                                  \
);                                                                            \
                                                                              \
template<class Form, class Cmpt, int nCmpt, class Type, class GeoMesh>        \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
  const VectorSpace<Form,Cmpt,nCmpt>& vs,                                     \
  const DimensionedField<Type, GeoMesh>& df1                                  \
);                                                                            \
                                                                              \
template<class Form, class Type, class GeoMesh>                               \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
  const dimensioned<Form>& dvs,                                               \
  const tmp<DimensionedField<Type, GeoMesh> >& tdf1                           \
);                                                                            \
                                                                              \
template<class Form, class Cmpt, int nCmpt, class Type, class GeoMesh>        \
tmp<DimensionedField<typename product<Form, Type>::type, GeoMesh> >           \
operator op                                                                   \
(                                                                             \
  const VectorSpace<Form,Cmpt,nCmpt>& vs,                                     \
  const tmp<DimensionedField<Type, GeoMesh> >& tdf1                           \
);

PRODUCT_OPERATOR(typeOfSum, +, add)
PRODUCT_OPERATOR(typeOfSum, -, subtract)
PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)

#undef PRODUCT_OPERATOR

}  // namespace mousse

#include "undef_field_functions_m.inc"
