// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dim_field_decomposer.hpp"
// Member Functions 
template<class Type>
mousse::tmp<mousse::DimensionedField<Type, mousse::volMesh>>
mousse::dimFieldDecomposer::decomposeField
(
  const DimensionedField<Type, volMesh>& field
) const
{
  // Create and map the internal field values
  Field<Type> mappedField(field, cellAddressing_);
  // Create the field for the processor
  return tmp<DimensionedField<Type, volMesh>>
  {
    new DimensionedField<Type, volMesh>
    {
      // IOobject
      {
        field.name(),
        procMesh_.time().timeName(),
        procMesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      procMesh_,
      field.dimensions(),
      mappedField
    }
  };
}
template<class GeoField>
void mousse::dimFieldDecomposer::decomposeFields
(
  const PtrList<GeoField>& fields
) const
{
  FOR_ALL(fields, fieldI)
  {
    decomposeField(fields[fieldI])().write();
  }
}
