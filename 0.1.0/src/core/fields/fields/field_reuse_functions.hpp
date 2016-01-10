// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef field_reuse_functions_hpp_
#define field_reuse_functions_hpp_
namespace mousse
{
template<class TypeR, class Type1>
class reuseTmp
{
public:
  static tmp<Field<TypeR> > New(const tmp<Field<Type1> >& tf1)
  {
    return tmp<Field<TypeR> >(new Field<TypeR>(tf1().size()));
  }
  static void clear(const tmp<Field<Type1> >& tf1)
  {
    tf1.clear();
  }
};
template<class TypeR>
class reuseTmp<TypeR, TypeR>
{
public:
  static tmp<Field<TypeR> > New(const tmp<Field<TypeR> >& tf1)
  {
    if (tf1.isTmp())
    {
      return tf1;
    }
    else
    {
      return tmp<Field<TypeR> >(new Field<TypeR>(tf1().size()));
    }
  }
  static void clear(const tmp<Field<TypeR> >& tf1)
  {
    if (tf1.isTmp())
    {
      tf1.ptr();
    }
  }
};
template<class TypeR, class Type1, class Type12, class Type2>
class reuseTmpTmp
{
public:
  static tmp<Field<TypeR> > New
  (
    const tmp<Field<Type1> >& tf1,
    const tmp<Field<Type2> >&
  )
  {
    return tmp<Field<TypeR> >(new Field<TypeR>(tf1().size()));
  }
  static void clear
  (
    const tmp<Field<Type1> >& tf1,
    const tmp<Field<Type2> >& tf2
  )
  {
    tf1.clear();
    tf2.clear();
  }
};
template<class TypeR, class Type1, class Type12>
class reuseTmpTmp<TypeR, Type1, Type12, TypeR>
{
public:
  static tmp<Field<TypeR> > New
  (
    const tmp<Field<Type1> >& tf1,
    const tmp<Field<TypeR> >& tf2
  )
  {
    if (tf2.isTmp())
    {
      return tf2;
    }
    else
    {
      return tmp<Field<TypeR> >(new Field<TypeR>(tf1().size()));
    }
  }
  static void clear
  (
    const tmp<Field<Type1> >& tf1,
    const tmp<Field<TypeR> >& tf2
  )
  {
    tf1.clear();
    if (tf2.isTmp())
    {
      tf2.ptr();
    }
  }
};
template<class TypeR, class Type2>
class reuseTmpTmp<TypeR, TypeR, TypeR, Type2>
{
public:
  static tmp<Field<TypeR> > New
  (
    const tmp<Field<TypeR> >& tf1,
    const tmp<Field<Type2> >&
  )
  {
    if (tf1.isTmp())
    {
      return tf1;
    }
    else
    {
      return tmp<Field<TypeR> >(new Field<TypeR>(tf1().size()));
    }
  }
  static void clear
  (
    const tmp<Field<TypeR> >& tf1,
    const tmp<Field<Type2> >& tf2
  )
  {
    if (tf1.isTmp())
    {
      tf1.ptr();
    }
    tf2.clear();
  }
};
template<class TypeR>
class reuseTmpTmp<TypeR, TypeR, TypeR, TypeR>
{
public:
  static tmp<Field<TypeR> > New
  (
    const tmp<Field<TypeR> >& tf1,
    const tmp<Field<TypeR> >& tf2
  )
  {
    if (tf1.isTmp())
    {
      return tf1;
    }
    else if (tf2.isTmp())
    {
      return tf2;
    }
    else
    {
      return tmp<Field<TypeR> >(new Field<TypeR>(tf1().size()));
    }
  }
  static void clear
  (
    const tmp<Field<TypeR> >& tf1,
    const tmp<Field<TypeR> >& tf2
  )
  {
    if (tf1.isTmp())
    {
      tf1.ptr();
      tf2.clear();
    }
    else if (tf2.isTmp())
    {
      tf1.clear();
      tf2.ptr();
    }
  }
};
}  // namespace mousse
#endif
