// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const SubList<Type>& list
)
:
  SubList<Type>(list)
{}
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const UList<Type>& list
)
:
  SubList<Type>(list, list.size())
{}
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const UList<Type>& list,
  const label subSize
)
:
  SubList<Type>(list, subSize)
{}
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const UList<Type>& list,
  const label subSize,
  const label startIndex
)
:
  SubList<Type>(list, subSize, startIndex)
{}
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const SubField<Type>& sfield
)
:
  refCount(),
  SubList<Type>(sfield)
{}
// Member Functions 
template<class Type>
inline const mousse::SubField<Type>& mousse::SubField<Type>::null()
{
  return NullObjectRef<SubField<Type> >();
}
template<class Type>
inline mousse::tmp<mousse::Field<typename mousse::SubField<Type>::cmptType> >
mousse::SubField<Type>::component
(
  const direction d
) const
{
  return (reinterpret_cast<const Field<Type>&>(*this)).component(d);
}
template<class Type>
inline mousse::tmp<mousse::Field<Type> > mousse::SubField<Type>::T() const
{
  return (reinterpret_cast<const Field<Type>&>(*this)).T();
}
// Member Operators 
template<class Type>
inline void mousse::SubField<Type>::operator=(const SubField<Type>& rhs)
{
  UList<Type>::operator=(rhs);
}
template<class Type>
inline mousse::SubField<Type>::operator const mousse::Field<Type>&() const
{
  return *reinterpret_cast< const Field<Type>* >(this);
}
