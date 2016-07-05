// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_upwind.hpp"
#include "fv_mesh.hpp"


template<class Type>
mousse::tmp<
mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh>>
mousse::linearUpwind<Type>::correction
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  const fvMesh& mesh = this->mesh();
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsfCorr
  {
    new GeometricField<Type, fvsPatchField, surfaceMesh>
    {
      {
        "linearUpwind::correction(" + vf.name() + ')',
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      dimensioned<Type>{vf.name(), vf.dimensions(), pTraits<Type>::zero}
    }
  };
  GeometricField<Type, fvsPatchField, surfaceMesh>& sfCorr = tsfCorr();
  const surfaceScalarField& faceFlux = this->faceFlux_;
  const labelList& owner = mesh.owner();
  const labelList& neighbour = mesh.neighbour();
  const volVectorField& C = mesh.C();
  const surfaceVectorField& Cf = mesh.Cf();
  tmp
  <
    GeometricField
    <
      typename outerProduct<vector, Type>::type,
      fvPatchField,
      volMesh
    >
  > tgradVf = gradScheme_().grad(vf, gradSchemeName_);
  const GeometricField
  <
    typename outerProduct<vector, Type>::type,
    fvPatchField,
    volMesh
  >& gradVf = tgradVf();
  FOR_ALL(faceFlux, facei) {
    label celli = (faceFlux[facei] > 0) ? owner[facei] : neighbour[facei];
    sfCorr[facei] = (Cf[facei] - C[celli]) & gradVf[celli];
  }
  typename GeometricField<Type, fvsPatchField, surfaceMesh>::
    GeometricBoundaryField& bSfCorr = sfCorr.boundaryField();
  FOR_ALL(bSfCorr, patchi) {
    fvsPatchField<Type>& pSfCorr = bSfCorr[patchi];
    if (pSfCorr.coupled()) {
      const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
      const vectorField& pCf = Cf.boundaryField()[patchi];
      const scalarField& pFaceFlux = faceFlux.boundaryField()[patchi];
      const Field<typename outerProduct<vector, Type>::type> pGradVfNei
      {
        gradVf.boundaryField()[patchi].patchNeighbourField()
      };
      // Build the d-vectors
      vectorField pd{Cf.boundaryField()[patchi].patch().delta()};
      FOR_ALL(pOwner, facei) {
        label own = pOwner[facei];
        if (pFaceFlux[facei] > 0) {
          pSfCorr[facei] = (pCf[facei] - C[own]) & gradVf[own];
        } else {
          pSfCorr[facei] =
            (pCf[facei] - pd[facei] - C[own]) & pGradVfNei[facei];
        }
      }
    }
  }
  return tsfCorr;
}


namespace mousse
{

MAKELIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(linearUpwind, scalar)
MAKELIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(linearUpwind, vector)

}

