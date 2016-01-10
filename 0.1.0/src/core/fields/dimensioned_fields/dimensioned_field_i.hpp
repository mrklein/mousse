// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type, class GeoMesh>
inline const mousse::DimensionedField<Type, GeoMesh>&
mousse::DimensionedField<Type, GeoMesh>::null()
{
  return NullObjectRef<DimensionedField<Type, GeoMesh> >();
}
template<class Type, class GeoMesh>
inline const typename GeoMesh::Mesh&
mousse::DimensionedField<Type, GeoMesh>::mesh() const
{
  return mesh_;
}
template<class Type, class GeoMesh>
inline const mousse::dimensionSet&
mousse::DimensionedField<Type, GeoMesh>::dimensions() const
{
  return dimensions_;
}
template<class Type, class GeoMesh>
inline mousse::dimensionSet&
mousse::DimensionedField<Type, GeoMesh>::dimensions()
{
  return dimensions_;
}
template<class Type, class GeoMesh>
inline const mousse::Field<Type>&
mousse::DimensionedField<Type, GeoMesh>::field() const
{
  return *this;
}
template<class Type, class GeoMesh>
inline mousse::Field<Type>&
mousse::DimensionedField<Type, GeoMesh>::field()
{
  return *this;
}
