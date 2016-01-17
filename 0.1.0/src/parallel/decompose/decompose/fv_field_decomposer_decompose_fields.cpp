// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_field_decomposer.hpp"
#include "processor_fv_patch_field.hpp"
#include "processor_fvs_patch_field.hpp"
#include "processor_cyclic_fv_patch_field.hpp"
#include "processor_cyclic_fvs_patch_field.hpp"
#include "empty_fv_patch_fields.hpp"
// Member Functions 
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::fvFieldDecomposer::decomposeField
(
  const GeometricField<Type, fvPatchField, volMesh>& field,
  const bool allowUnknownPatchFields
) const
{
  // 1. Create the complete field with dummy patch fields
  PtrList<fvPatchField<Type> > patchFields(boundaryAddressing_.size());
  FOR_ALL(boundaryAddressing_, patchi)
  {
    patchFields.set
    (
      patchi,
      fvPatchField<Type>::New
      (
        calculatedFvPatchField<Type>::typeName,
        procMesh_.boundary()[patchi],
        DimensionedField<Type, volMesh>::null()
      )
    );
  }
  // Create the field for the processor
  tmp<GeometricField<Type, fvPatchField, volMesh> > tresF
  (
    new GeometricField<Type, fvPatchField, volMesh>
    (
      IOobject
      (
        field.name(),
        procMesh_.time().timeName(),
        procMesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      procMesh_,
      field.dimensions(),
      Field<Type>(field.internalField(), cellAddressing_),
      patchFields
    )
  );
  GeometricField<Type, fvPatchField, volMesh>& resF = tresF();
  // 2. Change the fvPatchFields to the correct type using a mapper
  //  constructor (with reference to the now correct internal field)
  typename GeometricField<Type, fvPatchField, volMesh>::
    GeometricBoundaryField& bf = resF.boundaryField();
  FOR_ALL(bf, patchi)
  {
    if (patchFieldDecomposerPtrs_[patchi])
    {
      bf.set
      (
        patchi,
        fvPatchField<Type>::New
        (
          field.boundaryField()[boundaryAddressing_[patchi]],
          procMesh_.boundary()[patchi],
          resF.dimensionedInternalField(),
          *patchFieldDecomposerPtrs_[patchi]
        )
      );
    }
    else if (isA<processorCyclicFvPatch>(procMesh_.boundary()[patchi]))
    {
      bf.set
      (
        patchi,
        new processorCyclicFvPatchField<Type>
        (
          procMesh_.boundary()[patchi],
          resF.dimensionedInternalField(),
          Field<Type>
          (
            field.internalField(),
            *processorVolPatchFieldDecomposerPtrs_[patchi]
          )
        )
      );
    }
    else if (isA<processorFvPatch>(procMesh_.boundary()[patchi]))
    {
      bf.set
      (
        patchi,
        new processorFvPatchField<Type>
        (
          procMesh_.boundary()[patchi],
          resF.dimensionedInternalField(),
          Field<Type>
          (
            field.internalField(),
            *processorVolPatchFieldDecomposerPtrs_[patchi]
          )
        )
      );
    }
    else if (allowUnknownPatchFields)
    {
      bf.set
      (
        patchi,
        new emptyFvPatchField<Type>
        (
          procMesh_.boundary()[patchi],
          resF.dimensionedInternalField()
        )
      );
    }
    else
    {
      FATAL_ERROR_IN("fvFieldDecomposer::decomposeField()")
        << "Unknown type." << abort(FatalError);
    }
  }
  // Create the field for the processor
  return tresF;
}
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh> >
mousse::fvFieldDecomposer::decomposeField
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& field
) const
{
  labelList mapAddr
  (
    labelList::subList
    (
      faceAddressing_,
      procMesh_.nInternalFaces()
    )
  );
  FOR_ALL(mapAddr, i)
  {
    mapAddr[i] -= 1;
  }
  // Create and map the internal field values
  Field<Type> internalField
  (
    field.internalField(),
    mapAddr
  );
  // Problem with addressing when a processor patch picks up both internal
  // faces and faces from cyclic boundaries. This is a bit of a hack, but
  // I cannot find a better solution without making the internal storage
  // mechanism for surfaceFields correspond to the one of faces in polyMesh
  // (i.e. using slices)
  Field<Type> allFaceField(field.mesh().nFaces());
  FOR_ALL(field.internalField(), i)
  {
    allFaceField[i] = field.internalField()[i];
  }
  FOR_ALL(field.boundaryField(), patchi)
  {
    const Field<Type> & p = field.boundaryField()[patchi];
    const label patchStart = field.mesh().boundaryMesh()[patchi].start();
    FOR_ALL(p, i)
    {
      allFaceField[patchStart + i] = p[i];
    }
  }
  // 1. Create the complete field with dummy patch fields
  PtrList<fvsPatchField<Type> > patchFields(boundaryAddressing_.size());
  FOR_ALL(boundaryAddressing_, patchi)
  {
    patchFields.set
    (
      patchi,
      fvsPatchField<Type>::New
      (
        calculatedFvsPatchField<Type>::typeName,
        procMesh_.boundary()[patchi],
        DimensionedField<Type, surfaceMesh>::null()
      )
    );
  }
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tresF
  (
    new GeometricField<Type, fvsPatchField, surfaceMesh>
    (
      IOobject
      (
        field.name(),
        procMesh_.time().timeName(),
        procMesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      procMesh_,
      field.dimensions(),
      Field<Type>(field.internalField(), mapAddr),
      patchFields
    )
  );
  GeometricField<Type, fvsPatchField, surfaceMesh>& resF = tresF();
  // 2. Change the fvsPatchFields to the correct type using a mapper
  //  constructor (with reference to the now correct internal field)
  typename GeometricField<Type, fvsPatchField, surfaceMesh>::
    GeometricBoundaryField& bf = resF.boundaryField();
  FOR_ALL(boundaryAddressing_, patchi)
  {
    if (patchFieldDecomposerPtrs_[patchi])
    {
      bf.set
      (
        patchi,
        fvsPatchField<Type>::New
        (
          field.boundaryField()[boundaryAddressing_[patchi]],
          procMesh_.boundary()[patchi],
          resF.dimensionedInternalField(),
          *patchFieldDecomposerPtrs_[patchi]
        )
      );
    }
    else if (isA<processorCyclicFvPatch>(procMesh_.boundary()[patchi]))
    {
      bf.set
      (
        patchi,
        new processorCyclicFvsPatchField<Type>
        (
          procMesh_.boundary()[patchi],
          resF.dimensionedInternalField(),
          Field<Type>
          (
            allFaceField,
            *processorSurfacePatchFieldDecomposerPtrs_[patchi]
          )
        )
      );
    }
    else if (isA<processorFvPatch>(procMesh_.boundary()[patchi]))
    {
      bf.set
      (
        patchi,
        new processorFvsPatchField<Type>
        (
          procMesh_.boundary()[patchi],
          resF.dimensionedInternalField(),
          Field<Type>
          (
            allFaceField,
            *processorSurfacePatchFieldDecomposerPtrs_[patchi]
          )
        )
      );
    }
    else
    {
      FATAL_ERROR_IN("fvFieldDecomposer::decomposeField()")
        << "Unknown type." << abort(FatalError);
    }
  }
  // Create the field for the processor
  return tresF;
}
template<class GeoField>
void mousse::fvFieldDecomposer::decomposeFields
(
  const PtrList<GeoField>& fields
) const
{
  FOR_ALL(fields, fieldI)
  {
    decomposeField(fields[fieldI])().write();
  }
}
