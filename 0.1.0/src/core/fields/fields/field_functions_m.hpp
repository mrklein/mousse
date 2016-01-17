// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#define UNARY_FUNCTION(ReturnType, Type1, Func)                               \
                                                                              \
TEMPLATE                                                                      \
void Func(Field<ReturnType>& res, const UList<Type1>& f);                     \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func(const UList<Type1>& f);                          \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func(const tmp<Field<Type1> >& tf);

#define UNARY_OPERATOR(ReturnType, Type1, Op, OpFunc)                         \
                                                                              \
TEMPLATE                                                                      \
void OpFunc(Field<ReturnType>& res, const UList<Type1>& f);                   \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op(const UList<Type1>& f);                   \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op(const tmp<Field<Type1> >& tf);

#define BINARY_FUNCTION(ReturnType, Type1, Type2, Func)                       \
                                                                              \
TEMPLATE                                                                      \
void Func                                                                     \
(                                                                             \
  Field<ReturnType>& f,                                                       \
  const UList<Type1>& f1,                                                     \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
  const UList<Type1>& f1,                                                     \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
  const UList<Type1>& f1,                                                     \
  const tmp<Field<Type2> >& tf2                                               \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
  const tmp<Field<Type1> >& tf1,                                              \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
  const tmp<Field<Type1> >& tf1,                                              \
  const tmp<Field<Type2> >& tf2                                               \
);

#define BINARY_TYPE_FUNCTION_SF(ReturnType, Type1, Type2, Func)               \
                                                                              \
TEMPLATE                                                                      \
void Func                                                                     \
(                                                                             \
  Field<ReturnType>& f,                                                       \
  const Type1& s1,                                                            \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
  const Type1& s1,                                                            \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
  const Type1& s1,                                                            \
  const tmp<Field<Type2> >& tf2                                               \
);

#define BINARY_TYPE_FUNCTION_FS(ReturnType, Type1, Type2, Func)               \
                                                                              \
TEMPLATE                                                                      \
void Func                                                                     \
(                                                                             \
  Field<ReturnType>& f,                                                       \
  const UList<Type1>& f1,                                                     \
  const Type2& s2                                                             \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
  const UList<Type1>& f1,                                                     \
  const Type2& s2                                                             \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > Func                                                  \
(                                                                             \
  const tmp<Field<Type1> >& tf1,                                              \
  const Type2& s2                                                             \
);

#define BINARY_TYPE_FUNCTION(ReturnType, Type1, Type2, Func)                  \
  BINARY_TYPE_FUNCTION_SF(ReturnType, Type1, Type2, Func)                     \
  BINARY_TYPE_FUNCTION_FS(ReturnType, Type1, Type2, Func)
#define BINARY_OPERATOR(ReturnType, Type1, Type2, Op, OpFunc)                 \
                                                                              \
TEMPLATE                                                                      \
void OpFunc                                                                   \
(                                                                             \
  Field<ReturnType>& f,                                                       \
  const UList<Type1>& f1,                                                     \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
  const UList<Type1>& f1,                                                     \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
  const UList<Type1>& f1,                                                     \
  const tmp<Field<Type2> >& tf2                                               \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
  const tmp<Field<Type1> >& tf1,                                              \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
  const tmp<Field<Type1> >& tf1,                                              \
  const tmp<Field<Type2> >& tf2                                               \
);

#define BINARY_TYPE_OPERATOR_SF(ReturnType, Type1, Type2, Op, OpFunc)         \
                                                                              \
TEMPLATE                                                                      \
void OpFunc                                                                   \
(                                                                             \
  Field<ReturnType>& f,                                                       \
  const Type1& s1,                                                            \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
  const Type1& s1,                                                            \
  const UList<Type2>& f2                                                      \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
  const Type1& s1,                                                            \
  const tmp<Field<Type2> >& tf2                                               \
);

#define BINARY_TYPE_OPERATOR_FS(ReturnType, Type1, Type2, Op, OpFunc)         \
                                                                              \
TEMPLATE                                                                      \
void OpFunc                                                                   \
(                                                                             \
  Field<ReturnType>& f,                                                       \
  const UList<Type1>& f1,                                                     \
  const Type2& s2                                                             \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
  const UList<Type1>& f1,                                                     \
  const Type2& s2                                                             \
);                                                                            \
                                                                              \
TEMPLATE                                                                      \
tmp<Field<ReturnType> > operator Op                                           \
(                                                                             \
  const tmp<Field<Type1> >& tf1,                                              \
  const Type2& s2                                                             \
);

#define BINARY_TYPE_OPERATOR(ReturnType, Type1, Type2, Op, OpFunc)            \
  BINARY_TYPE_OPERATOR_SF(ReturnType, Type1, Type2, Op, OpFunc)               \
  BINARY_TYPE_OPERATOR_FS(ReturnType, Type1, Type2, Op, OpFunc)
