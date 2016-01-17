// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_field_decomposer.hpp"
#include "processor_point_patch_fields.hpp"
// Member Functions 
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::pointPatchField, mousse::pointMesh>>
mousse::pointFieldDecomposer::decomposeField
(
  const GeometricField<Type, pointPatchField, pointMesh>& field
) const
{
  // Create and map the internal field values
  Field<Type> internalField(field.internalField(), pointAddressing_);
  // Create a list of pointers for the patchFields
  PtrList<pointPatchField<Type>> patchFields(boundaryAddressing_.size());
  // Create and map the patch field values
  FOR_ALL(boundaryAddressing_, patchi)
  {
    if (patchFieldDecomposerPtrs_[patchi])
    {
      patchFields.set
      (
        patchi,
        pointPatchField<Type>::New
        (
          field.boundaryField()[boundaryAddressing_[patchi]],
          procMesh_.boundary()[patchi],
          DimensionedField<Type, pointMesh>::null(),
          *patchFieldDecomposerPtrs_[patchi]
        )
      );
    }
    else
    {
      patchFields.set
      (
        patchi,
        new processorPointPatchField<Type>
        {
          procMesh_.boundary()[patchi],
          DimensionedField<Type, pointMesh>::null()
        }
      );
    }
  }
  // Create the field for the processor
  return tmp<GeometricField<Type, pointPatchField, pointMesh>>
  {
    new GeometricField<Type, pointPatchField, pointMesh>
    {
      // IOobject
      {
        field.name(),
        procMesh_().time().timeName(),
        procMesh_(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      procMesh_,
      field.dimensions(),
      internalField,
      patchFields
    }
  };
}
template<class GeoField>
void mousse::pointFieldDecomposer::decomposeFields
(
  const PtrList<GeoField>& fields
) const
{
  FOR_ALL(fields, fieldI)
  {
    decomposeField(fields[fieldI])().write();
  }
}
