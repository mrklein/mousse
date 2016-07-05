// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_cell_to_face_stencil.hpp"
// Member Functions 
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh> >
mousse::extendedUpwindCellToFaceStencil::weightedSum
(
  const surfaceScalarField& phi,
  const GeometricField<Type, fvPatchField, volMesh>& fld,
  const List<List<scalar> >& ownWeights,
  const List<List<scalar> >& neiWeights
) const
{
  const fvMesh& mesh = fld.mesh();
  // Collect internal and boundary values
  List<List<Type> > ownFld;
  collectData(ownMap(), ownStencil(), fld, ownFld);
  List<List<Type> > neiFld;
  collectData(neiMap(), neiStencil(), fld, neiFld);
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
    if (phi[faceI] > 0)
    {
      // Flux out of owner. Use upwind (= owner side) stencil.
      const List<Type>& stField = ownFld[faceI];
      const List<scalar>& stWeight = ownWeights[faceI];
      FOR_ALL(stField, i)
      {
        sf[faceI] += stField[i]*stWeight[i];
      }
    }
    else
    {
      const List<Type>& stField = neiFld[faceI];
      const List<scalar>& stWeight = neiWeights[faceI];
      FOR_ALL(stField, i)
      {
        sf[faceI] += stField[i]*stWeight[i];
      }
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
        if (phi.boundaryField()[patchi][i] > 0)
        {
          // Flux out of owner. Use upwind (= owner side) stencil.
          const List<Type>& stField = ownFld[faceI];
          const List<scalar>& stWeight = ownWeights[faceI];
          FOR_ALL(stField, j)
          {
            pSfCorr[i] += stField[j]*stWeight[j];
          }
        }
        else
        {
          const List<Type>& stField = neiFld[faceI];
          const List<scalar>& stWeight = neiWeights[faceI];
          FOR_ALL(stField, j)
          {
            pSfCorr[i] += stField[j]*stWeight[j];
          }
        }
        faceI++;
      }
    }
  }
  return tsfCorr;
}
