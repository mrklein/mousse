// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_face_to_cell_stencil.hpp"
// Member Functions 
template<class Type>
void mousse::extendedFaceToCellStencil::collectData
(
  const mapDistribute& map,
  const labelListList& stencil,
  const GeometricField<Type, fvsPatchField, surfaceMesh>& fld,
  List<List<Type> >& stencilFld
)
{
  // 1. Construct face data in compact addressing
  List<Type> flatFld(map.constructSize(), pTraits<Type>::zero);
  // Insert my internal values
  FOR_ALL(fld, cellI)
  {
    flatFld[cellI] = fld[cellI];
  }
  // Insert my boundary values
  FOR_ALL(fld.boundaryField(), patchI)
  {
    const fvsPatchField<Type>& pfld = fld.boundaryField()[patchI];
    label nCompact = pfld.patch().start();
    FOR_ALL(pfld, i)
    {
      flatFld[nCompact++] = pfld[i];
    }
  }
  // Do all swapping
  map.distribute(flatFld);
  // 2. Pull to stencil
  stencilFld.setSize(stencil.size());
  FOR_ALL(stencil, faceI)
  {
    const labelList& compactCells = stencil[faceI];
    stencilFld[faceI].setSize(compactCells.size());
    FOR_ALL(compactCells, i)
    {
      stencilFld[faceI][i] = flatFld[compactCells[i]];
    }
  }
}
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::extendedFaceToCellStencil::weightedSum
(
  const mapDistribute& map,
  const labelListList& stencil,
  const GeometricField<Type, fvsPatchField, surfaceMesh>& fld,
  const List<List<scalar> >& stencilWeights
)
{
  const fvMesh& mesh = fld.mesh();
  // Collect internal and boundary values
  List<List<Type> > stencilFld;
  collectData(map, stencil, fld, stencilFld);
  tmp<GeometricField<Type, fvPatchField, volMesh> > tsfCorr
  (
    new GeometricField<Type, fvPatchField, volMesh>
    (
      IOobject
      (
        fld.name(),
        mesh.time().timeName(),
        mesh
      ),
      mesh,
      dimensioned<Type>
      (
        fld.name(),
        fld.dimensions(),
        pTraits<Type>::zero
      )
    )
  );
  GeometricField<Type, fvPatchField, volMesh>& sf = tsfCorr();
  // cells
  FOR_ALL(sf, cellI)
  {
    const List<Type>& stField = stencilFld[cellI];
    const List<scalar>& stWeight = stencilWeights[cellI];
    FOR_ALL(stField, i)
    {
      sf[cellI] += stField[i]*stWeight[i];
    }
  }
  // Boundaries values?
  return tsfCorr;
}
