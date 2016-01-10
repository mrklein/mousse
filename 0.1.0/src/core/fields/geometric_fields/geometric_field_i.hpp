// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type, template<class> class PatchField, class GeoMesh>
inline const mousse::GeometricField<Type, PatchField, GeoMesh>&
mousse::GeometricField<Type, PatchField, GeoMesh>::null()
{
  return NullObjectRef<GeometricField<Type, PatchField, GeoMesh> >();
}
template<class Type, template<class> class PatchField, class GeoMesh>
inline
const typename
mousse::GeometricField<Type, PatchField, GeoMesh>::DimensionedInternalField&
mousse::GeometricField<Type, PatchField, GeoMesh>::
dimensionedInternalField() const
{
  return *this;
}
template<class Type, template<class> class PatchField, class GeoMesh>
inline
const typename
mousse::GeometricField<Type, PatchField, GeoMesh>::InternalField&
mousse::GeometricField<Type, PatchField, GeoMesh>::internalField() const
{
  return *this;
}
template<class Type, template<class> class PatchField, class GeoMesh>
inline const typename mousse::GeometricField<Type, PatchField, GeoMesh>::
GeometricBoundaryField&
mousse::GeometricField<Type, PatchField, GeoMesh>::boundaryField() const
{
  return boundaryField_;
}
template<class Type, template<class> class PatchField, class GeoMesh>
inline mousse::label
mousse::GeometricField<Type, PatchField, GeoMesh>::timeIndex() const
{
  return timeIndex_;
}
template<class Type, template<class> class PatchField, class GeoMesh>
inline mousse::label&
mousse::GeometricField<Type, PatchField, GeoMesh>::timeIndex()
{
  return timeIndex_;
}
