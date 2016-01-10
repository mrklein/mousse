// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
inline mousse::label
mousse::interpolationLookUpTable<Type>::findFieldIndex
(
  const word& fieldName
) const
{
  return fieldIndices_[fieldName];
}
template<class Type>
inline const mousse::List<mousse::dictionary>&
mousse::interpolationLookUpTable<Type>::output() const
{
  return output_;
}
template<class Type>
inline const mousse::List<mousse::dictionary>&
mousse::interpolationLookUpTable<Type>::entries() const
{
  return entries_;
}
template<class Type>
inline const mousse::List<mousse::scalar>&
mousse::interpolationLookUpTable<Type>::min() const
{
  return min_;
}
template<class Type>
inline const mousse::List<mousse::label>&
mousse::interpolationLookUpTable<Type>::dim() const
{
  return dim_;
}
template<class Type>
inline const mousse::List<mousse::scalar>&
mousse::interpolationLookUpTable<Type>::delta() const
{
  return delta_;
}
template<class Type>
inline const mousse::List<mousse::scalar>&
mousse::interpolationLookUpTable<Type>::max() const
{
  return max_;
}
template<class Type>
inline mousse::word mousse::interpolationLookUpTable<Type>::tableName() const
{
  return fileName_.name();
}
