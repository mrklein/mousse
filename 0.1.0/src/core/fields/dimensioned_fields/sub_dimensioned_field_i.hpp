// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
  const SubField<Type>& slist
)
:
  SubField<Type>(slist)
{}
template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
  const UList<Type>& list,
  const label subSize
)
:
  SubField<Type>(list, subSize)
{}
template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
  const UList<Type>& list,
  const label subSize,
  const label startIndex
)
:
  SubField<Type>(list, subSize, startIndex)
{}
template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
  const SubDimensionedField<Type, GeoMesh>& sfield
)
:
  refCount(),
  SubField<Type>(sfield)
{}
// Member Functions 
template<class Type, class GeoMesh>
inline const mousse::SubDimensionedField<Type, GeoMesh>&
mousse::SubDimensionedField<Type, GeoMesh>::null()
{
  return NullObjectRef<SubDimensionedField<Type, GeoMesh> >();
}
template<class Type, class GeoMesh>
inline
mousse::tmp
<
  mousse::Field<typename mousse::SubDimensionedField<Type, GeoMesh>::cmptType>
>
mousse::SubDimensionedField<Type, GeoMesh>::component
(
  const direction d
) const
{
  return
  (
    reinterpret_cast<const DimensionedField<Type, GeoMesh>&>(*this)
  ).component(d);
}
template<class Type, class GeoMesh>
inline mousse::tmp<mousse::DimensionedField<Type, GeoMesh> >
mousse::SubDimensionedField<Type, GeoMesh>::T() const
{
  return
  (
    reinterpret_cast<const DimensionedField<Type, GeoMesh>&>(*this)
  ).T();
}
// Member Operators 
template<class Type, class GeoMesh>
inline void mousse::SubDimensionedField<Type, GeoMesh>::operator=
(
  const SubDimensionedField<Type, GeoMesh>& rhs
)
{
  dimensions() = rhs.dimensions();
  SubField<Type>::operator=(rhs);
}
template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::operator
const mousse::DimensionedField<Type, GeoMesh>&() const
{
  return *(reinterpret_cast<const DimensionedField<Type, GeoMesh>*>(this));
}
