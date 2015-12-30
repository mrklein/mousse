// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_patch_internal_field.hpp"
#include "interpolation_cell_point.hpp"
#include "primitive_patch_interpolation.hpp"
// Private Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::sampledPatchInternalField::sampleField
(
  const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
  // One value per face
  tmp<Field<Type> > tvalues(new Field<Type>(patchFaceLabels().size()));
  Field<Type>& values = tvalues();
  forAll(patchStart(), i)
  {
    // Get patchface wise data by sampling internal field
    Field<Type> interpVals = vField.internalField();
    mappers_[i].map().distribute(interpVals);
    // Store at correct position in values
    label end =
    (
      i < patchStart().size()-1
     ? patchStart()[i+1]
     : patchFaceLabels().size()
    );
    for (label triI = patchStart()[i]; triI < end; triI++)
    {
      values[triI] = interpVals[patchFaceLabels()[triI]];
    }
  }
  return tvalues;
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::sampledPatchInternalField::interpolateField
(
  const interpolation<Type>& interpolator
) const
{
  label sz = 0;
  forAll(patchIDs(), i)
  {
    sz += mesh().boundaryMesh()[patchIDs()[i]].size();
  }
  Field<Type> allPatchVals(sz);
  sz = 0;
  forAll(patchIDs(), i)
  {
    // See mappedFixedValueFvPatchField
    const mapDistribute& distMap = mappers_[i].map();
    // Send back sample points to processor that holds the cell.
    // Mark cells with point::max so we know which ones we need
    // to interpolate (since expensive).
    vectorField samples(mappers_[i].samplePoints());
    distMap.reverseDistribute(mesh().nCells(), point::max, samples);
    Field<Type> patchVals(mesh().nCells());
    forAll(samples, cellI)
    {
      if (samples[cellI] != point::max)
      {
        patchVals[cellI] = interpolator.interpolate
        (
          samples[cellI],
          cellI
        );
      }
    }
    distMap.distribute(patchVals);
    // Now patchVals holds the interpolated data in patch face order.
    // Collect.
    SubList<Type>(allPatchVals, patchVals.size(), sz).assign(patchVals);
    sz += patchVals.size();
  }
  // Interpolate to points. Reconstruct the patch of all faces to aid
  // interpolation.
  labelList meshFaceLabels(allPatchVals.size());
  sz = 0;
  forAll(patchIDs(), i)
  {
    const polyPatch& pp = mesh().boundaryMesh()[patchIDs()[i]];
    forAll(pp, i)
    {
      meshFaceLabels[sz++] = pp.start()+i;
    }
  }
  indirectPrimitivePatch allPatches
  (
    IndirectList<face>(mesh().faces(), meshFaceLabels),
    mesh().points()
  );
  return PrimitivePatchInterpolation<indirectPrimitivePatch>
  (
    allPatches
  ).faceToPointInterpolate(allPatchVals);
}
