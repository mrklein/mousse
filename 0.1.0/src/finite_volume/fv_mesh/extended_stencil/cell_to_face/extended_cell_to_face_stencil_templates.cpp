// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_cell_to_face_stencil.hpp"
// Member Functions 
template<class Type>
void mousse::extendedCellToFaceStencil::collectData
(
  const mapDistribute& map,
  const labelListList& stencil,
  const GeometricField<Type, fvPatchField, volMesh>& fld,
  List<List<Type> >& stencilFld
)
{
  // 1. Construct cell data in compact addressing
  List<Type> flatFld(map.constructSize(), pTraits<Type>::zero);
  // Insert my internal values
  FOR_ALL(fld, cellI)
  {
    flatFld[cellI] = fld[cellI];
  }
  // Insert my boundary values
  FOR_ALL(fld.boundaryField(), patchI)
  {
    const fvPatchField<Type>& pfld = fld.boundaryField()[patchI];
    label nCompact =
      pfld.patch().start()
     -fld.mesh().nInternalFaces()
     +fld.mesh().nCells();
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
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh> >
mousse::extendedCellToFaceStencil::weightedSum
(
  const mapDistribute& map,
  const labelListList& stencil,
  const GeometricField<Type, fvPatchField, volMesh>& fld,
  const List<List<scalar> >& stencilWeights
)
{
  const fvMesh& mesh = fld.mesh();
  // Collect internal and boundary values
  List<List<Type> > stencilFld;
  collectData(map, stencil, fld, stencilFld);
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsfCorr
  (
    new GeometricField<Type, fvsPatchField, surfaceMesh>
    (
      IOobject
      (
        fld.name(),
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
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
  GeometricField<Type, fvsPatchField, surfaceMesh>& sf = tsfCorr();
  // Internal faces
  for (label faceI = 0; faceI < mesh.nInternalFaces(); faceI++)
  {
    const List<Type>& stField = stencilFld[faceI];
    const List<scalar>& stWeight = stencilWeights[faceI];
    FOR_ALL(stField, i)
    {
      sf[faceI] += stField[i]*stWeight[i];
    }
  }
  // Boundaries. Either constrained or calculated so assign value
  // directly (instead of nicely using operator==)
  typename GeometricField<Type, fvsPatchField, surfaceMesh>::
    GeometricBoundaryField& bSfCorr = sf.boundaryField();
  FOR_ALL(bSfCorr, patchi)
  {
    fvsPatchField<Type>& pSfCorr = bSfCorr[patchi];
    if (pSfCorr.coupled())
    {
      label faceI = pSfCorr.patch().start();
      FOR_ALL(pSfCorr, i)
      {
        const List<Type>& stField = stencilFld[faceI];
        const List<scalar>& stWeight = stencilWeights[faceI];
        FOR_ALL(stField, j)
        {
          pSfCorr[i] += stField[j]*stWeight[j];
        }
        faceI++;
      }
    }
  }
  return tsfCorr;
}
