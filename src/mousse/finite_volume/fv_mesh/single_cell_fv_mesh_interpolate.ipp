// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "single_cell_fv_mesh.hpp"
#include "calculated_fv_patch_fields.hpp"
#include "direct_fv_patch_field_mapper.hpp"
#include "time.hpp"


namespace mousse {

// Member Functions 
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> singleCellFvMesh::interpolate
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  // 1. Create the complete field with dummy patch fields
  PtrList<fvPatchField<Type>> patchFields{vf.boundaryField().size()};
  FOR_ALL(patchFields, patchI) {
    patchFields.set
    (
      patchI,
      fvPatchField<Type>::New
      (
        calculatedFvPatchField<Type>::typeName,
        boundary()[patchI],
        DimensionedField<Type, volMesh>::null()
      )
    );
  }
  // Create the complete field from the pieces
  tmp<GeometricField<Type, fvPatchField, volMesh>> tresF
  {
    new GeometricField<Type, fvPatchField, volMesh>
    {
      {
        vf.name(),
        time().timeName(),
        *this,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      *this,
      vf.dimensions(),
      Field<Type>{1, gAverage(vf)},
      patchFields
    }
  };
  GeometricField<Type, fvPatchField, volMesh>& resF = tresF();
  // 2. Change the fvPatchFields to the correct type using a mapper
  //  constructor (with reference to the now correct internal field)
  typename GeometricField<Type, fvPatchField, volMesh>::
    GeometricBoundaryField& bf = resF.boundaryField();
  if (agglomerate()) {
    FOR_ALL(vf.boundaryField(), patchI) {
      const labelList& agglom = patchFaceAgglomeration_[patchI];
      label nAgglom = max(agglom) + 1;
      // Use inverse of agglomeration. This is from agglomeration to
      // original (fine) mesh patch face.
      labelListList coarseToFine{invertOneToMany(nAgglom, agglom)};
      inplaceReorder(patchFaceMap_[patchI], coarseToFine);
      scalarListList coarseWeights{nAgglom};
      FOR_ALL(coarseToFine, coarseI) {
        const labelList& fineFaces = coarseToFine[coarseI];
        coarseWeights[coarseI] = scalarList
        {
          fineFaces.size(),
          1.0/fineFaces.size()
        };
      }
      bf.set
      (
        patchI,
        fvPatchField<Type>::New
        (
          vf.boundaryField()[patchI],
          boundary()[patchI],
          resF.dimensionedInternalField(),
          agglomPatchFieldMapper(coarseToFine, coarseWeights)
        )
      );
    }
  } else {
    FOR_ALL(vf.boundaryField(), patchI) {
      labelList map(identity(vf.boundaryField()[patchI].size()));
      bf.set
      (
        patchI,
        fvPatchField<Type>::New
        (
          vf.boundaryField()[patchI],
          boundary()[patchI],
          resF.dimensionedInternalField(),
          directFvPatchFieldMapper(map)
        )
      );
    }
  }
  return tresF;
}

}  // namespace mousse
