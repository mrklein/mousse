// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_semi_implicit_source.hpp"
// Member Functions 
template<class Type>
inline const typename mousse::fv::SemiImplicitSource<Type>::volumeModeType&
mousse::fv::SemiImplicitSource<Type>::volumeMode() const
{
  return volumeMode_;
}
template<class Type>
inline const mousse::List<mousse::Tuple2<Type, mousse::scalar> >&
mousse::fv::SemiImplicitSource<Type>::injectionRate() const
{
  return injectionRate_;
}
template<class Type>
inline typename mousse::fv::SemiImplicitSource<Type>::volumeModeType&
mousse::fv::SemiImplicitSource<Type>::volumeMode()
{
  return volumeMode_;
}
template<class Type>
inline mousse::List<mousse::Tuple2<Type,
mousse::scalar> >& mousse::fv::SemiImplicitSource<Type>::injectionRate()
{
  return injectionRate_;
}
