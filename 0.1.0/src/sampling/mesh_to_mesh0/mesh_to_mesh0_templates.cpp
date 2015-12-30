// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_to_mesh0.hpp"
#include "vol_fields.hpp"
#include "interpolation_cell_point.hpp"
#include "sub_field.hpp"
#include "mixed_fv_patch_field.hpp"
// Member Functions 
template<class Type, class CombineOp>
void mousse::meshToMesh0::mapField
(
  Field<Type>& toF,
  const Field<Type>& fromVf,
  const labelList& adr,
  const CombineOp& cop
) const
{
  // Direct mapping of nearest-cell values
  forAll(toF, celli)
  {
    if (adr[celli] != -1)
    {
      cop(toF[celli], fromVf[adr[celli]]);
    }
  }
  //toF.map(fromVf, adr);
}
template<class Type, class CombineOp>
void mousse::meshToMesh0::interpolateField
(
  Field<Type>& toF,
  const GeometricField<Type, fvPatchField, volMesh>& fromVf,
  const labelListList& adr,
  const scalarListList& weights,
  const CombineOp& cop
) const
{
  // Inverse volume weighted interpolation
  forAll(toF, celli)
  {
    const labelList& overlapCells = adr[celli];
    const scalarList& w = weights[celli];
    Type f = pTraits<Type>::zero;
    forAll(overlapCells, i)
    {
      label fromCelli = overlapCells[i];
      f += fromVf[fromCelli]*w[i];
      cop(toF[celli], f);
    }
  }
}
template<class Type, class CombineOp>
void mousse::meshToMesh0::interpolateField
(
  Field<Type>& toF,
  const GeometricField<Type, fvPatchField, volMesh>& fromVf,
  const labelList& adr,
  const scalarListList& weights,
  const CombineOp& cop
) const
{
  // Inverse distance weighted interpolation
  // get reference to cellCells
  const labelListList& cc = fromMesh_.cellCells();
  forAll(toF, celli)
  {
    if (adr[celli] != -1)
    {
      const labelList& neighbours = cc[adr[celli]];
      const scalarList& w = weights[celli];
      Type f = fromVf[adr[celli]]*w[0];
      for (label ni = 1; ni < w.size(); ni++)
      {
        f += fromVf[neighbours[ni - 1]]*w[ni];
      }
      cop(toF[celli], f);
    }
  }
}
template<class Type, class CombineOp>
void mousse::meshToMesh0::interpolateField
(
  Field<Type>& toF,
  const GeometricField<Type, fvPatchField, volMesh>& fromVf,
  const labelList& adr,
  const vectorField& centres,
  const CombineOp& cop
) const
{
  // Cell-Point interpolation
  interpolationCellPoint<Type> interpolator(fromVf);
  forAll(toF, celli)
  {
    if (adr[celli] != -1)
    {
      cop
      (
        toF[celli],
        interpolator.interpolate
        (
          centres[celli],
          adr[celli]
        )
      );
    }
  }
}
template<class Type, class CombineOp>
void mousse::meshToMesh0::interpolateInternalField
(
  Field<Type>& toF,
  const GeometricField<Type, fvPatchField, volMesh>& fromVf,
  meshToMesh0::order ord,
  const CombineOp& cop
) const
{
  if (fromVf.mesh() != fromMesh_)
  {
    FatalErrorIn
    (
      "meshToMesh0::interpolateInternalField(Field<Type>&, "
      "const GeometricField<Type, fvPatchField, volMesh>&, "
      "meshToMesh0::order, const CombineOp&) const"
    )   << "the argument field does not correspond to the right mesh. "
      << "Field size: " << fromVf.size()
      << " mesh size: " << fromMesh_.nCells()
      << exit(FatalError);
  }
  if (toF.size() != toMesh_.nCells())
  {
    FatalErrorIn
    (
      "meshToMesh0::interpolateInternalField(Field<Type>&, "
      "const GeometricField<Type, fvPatchField, volMesh>&, "
      "meshToMesh0::order, const CombineOp&) const"
    )   << "the argument field does not correspond to the right mesh. "
      << "Field size: " << toF.size()
      << " mesh size: " << toMesh_.nCells()
      << exit(FatalError);
  }
  switch(ord)
  {
    case MAP:
      mapField(toF, fromVf, cellAddressing_, cop);
    break;
    case INTERPOLATE:
    {
      interpolateField
      (
        toF,
        fromVf,
        cellAddressing_,
        inverseDistanceWeights(),
        cop
      );
      break;
    }
    case CELL_POINT_INTERPOLATE:
    {
      interpolateField
      (
        toF,
        fromVf,
        cellAddressing_,
        toMesh_.cellCentres(),
        cop
      );
      break;
    }
    case CELL_VOLUME_WEIGHT:
    {
      const labelListList& cellToCell = cellToCellAddressing();
      const scalarListList& invVolWeights = inverseVolumeWeights();
      interpolateField
      (
        toF,
        fromVf,
        cellToCell,
        invVolWeights,
        cop
      );
      break;
    }
    default:
      FatalErrorIn
      (
        "meshToMesh0::interpolateInternalField(Field<Type>&, "
        "const GeometricField<Type, fvPatchField, volMesh>&, "
        "meshToMesh0::order, const CombineOp&) const"
      )   << "unknown interpolation scheme " << ord
        << exit(FatalError);
  }
}
template<class Type, class CombineOp>
void mousse::meshToMesh0::interpolateInternalField
(
  Field<Type>& toF,
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfromVf,
  meshToMesh0::order ord,
  const CombineOp& cop
) const
{
  interpolateInternalField(toF, tfromVf(), ord, cop);
  tfromVf.clear();
}
template<class Type, class CombineOp>
void mousse::meshToMesh0::interpolate
(
  GeometricField<Type, fvPatchField, volMesh>& toVf,
  const GeometricField<Type, fvPatchField, volMesh>& fromVf,
  meshToMesh0::order ord,
  const CombineOp& cop
) const
{
  interpolateInternalField(toVf, fromVf, ord, cop);
  forAll(toMesh_.boundaryMesh(), patchi)
  {
    const fvPatch& toPatch = toMesh_.boundary()[patchi];
    if (cuttingPatches_.found(toPatch.name()))
    {
      switch(ord)
      {
        case MAP:
        {
          mapField
          (
            toVf.boundaryField()[patchi],
            fromVf,
            boundaryAddressing_[patchi],
            cop
          );
          break;
        }
        case INTERPOLATE:
        {
          interpolateField
          (
            toVf.boundaryField()[patchi],
            fromVf,
            boundaryAddressing_[patchi],
            toPatch.Cf(),
            cop
          );
          break;
        }
        case CELL_POINT_INTERPOLATE:
        {
          interpolateField
          (
            toVf.boundaryField()[patchi],
            fromVf,
            boundaryAddressing_[patchi],
            toPatch.Cf(),
            cop
          );
          break;
        }
        case CELL_VOLUME_WEIGHT:
        {
          // Do nothing
          break;
        }
        default:
          FatalErrorIn
          (
            "meshToMesh0::interpolate("
            "GeometricField<Type, fvPatchField, volMesh>&, "
            "const GeometricField<Type, fvPatchField, volMesh>&, "
            "meshToMesh0::order, const CombineOp&) const"
          )   << "unknown interpolation scheme " << ord
            << exit(FatalError);
      }
      if (isA<mixedFvPatchField<Type> >(toVf.boundaryField()[patchi]))
      {
        refCast<mixedFvPatchField<Type> >
        (
          toVf.boundaryField()[patchi]
        ).refValue() = toVf.boundaryField()[patchi];
      }
    }
    else if
    (
      patchMap_.found(toPatch.name())
    && fromMeshPatches_.found(patchMap_.find(toPatch.name())())
    )
    {
      /*
      toVf.boundaryField()[patchi].map
      (
        fromVf.boundaryField()
        [
          fromMeshPatches_.find(patchMap_.find(toPatch.name())())()
        ],
        boundaryAddressing_[patchi]
      );
      */
      mapField
      (
        toVf.boundaryField()[patchi],
        fromVf.boundaryField()
        [
          fromMeshPatches_.find(patchMap_.find(toPatch.name())())()
        ],
        boundaryAddressing_[patchi],
        cop
      );
    }
  }
}
template<class Type, class CombineOp>
void mousse::meshToMesh0::interpolate
(
  GeometricField<Type, fvPatchField, volMesh>& toVf,
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfromVf,
  meshToMesh0::order ord,
  const CombineOp& cop
) const
{
  interpolate(toVf, tfromVf(), ord, cop);
  tfromVf.clear();
}
template<class Type, class CombineOp>
mousse::tmp< mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh0::interpolate
(
  const GeometricField<Type, fvPatchField, volMesh>& fromVf,
  meshToMesh0::order ord,
  const CombineOp& cop
) const
{
  // Create and map the internal-field values
  Field<Type> internalField(toMesh_.nCells());
  interpolateInternalField(internalField, fromVf, ord, cop);
  // check whether both meshes have got the same number
  // of boundary patches
  if (fromMesh_.boundary().size() != toMesh_.boundary().size())
  {
    FatalErrorIn
    (
      "meshToMesh0::interpolate"
      "(const GeometricField<Type, fvPatchField, volMesh>&,"
      "meshToMesh0::order, const CombineOp&) const"
    )   << "Incompatible meshes: different number of boundaries, "
       "only internal field may be interpolated"
      << exit(FatalError);
  }
  // Create and map the patch field values
  PtrList<fvPatchField<Type> > patchFields
  (
    boundaryAddressing_.size()
  );
  forAll(boundaryAddressing_, patchI)
  {
    patchFields.set
    (
      patchI,
      fvPatchField<Type>::New
      (
        fromVf.boundaryField()[patchI],
        toMesh_.boundary()[patchI],
        DimensionedField<Type, volMesh>::null(),
        patchFieldInterpolator
        (
          boundaryAddressing_[patchI]
        )
      )
    );
  }
  // Create the complete field from the pieces
  tmp<GeometricField<Type, fvPatchField, volMesh> > ttoF
  (
    new GeometricField<Type, fvPatchField, volMesh>
    (
      IOobject
      (
        "interpolated(" + fromVf.name() + ')',
        toMesh_.time().timeName(),
        toMesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      toMesh_,
      fromVf.dimensions(),
      internalField,
      patchFields
    )
  );
  return ttoF;
}
template<class Type, class CombineOp>
mousse::tmp< mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh0::interpolate
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfromVf,
  meshToMesh0::order ord,
  const CombineOp& cop
) const
{
  tmp<GeometricField<Type, fvPatchField, volMesh> > tint =
    interpolate(tfromVf(), ord, cop);
  tfromVf.clear();
  return tint;
}
