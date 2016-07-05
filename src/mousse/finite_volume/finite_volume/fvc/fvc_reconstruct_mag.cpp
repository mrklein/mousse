// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_reconstruct.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"


namespace mousse {
namespace fvc {

tmp<volScalarField> reconstructMag(const surfaceScalarField& ssf)
{
  const fvMesh& mesh = ssf.mesh();
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  const volVectorField& C = mesh.C();
  const surfaceVectorField& Cf = mesh.Cf();
  const surfaceVectorField& Sf = mesh.Sf();
  const surfaceScalarField& magSf = mesh.magSf();
  tmp<volScalarField> treconField
  {
    new volScalarField
    {
      {
        "reconstruct("+ssf.name()+')',
        ssf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      {"0", ssf.dimensions()/dimArea, scalar(0)},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  scalarField& rf = treconField();
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    rf[own] += (Sf[facei] & (Cf[facei] - C[own]))*ssf[facei]/magSf[facei];
    rf[nei] -= (Sf[facei] & (Cf[facei] - C[nei]))*ssf[facei]/magSf[facei];
  }
  const surfaceScalarField::GeometricBoundaryField& bsf = ssf.boundaryField();
  FOR_ALL(bsf, patchi) {
    const fvsPatchScalarField& psf = bsf[patchi];
    const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
    const vectorField& pCf = Cf.boundaryField()[patchi];
    const vectorField& pSf = Sf.boundaryField()[patchi];
    const scalarField& pMagSf = magSf.boundaryField()[patchi];
    FOR_ALL(pOwner, pFacei) {
      label own = pOwner[pFacei];
      rf[own] +=
        (pSf[pFacei] & (pCf[pFacei] - C[own]))*psf[pFacei]/pMagSf[pFacei];
    }
  }
  rf /= mesh.V();
  treconField().correctBoundaryConditions();
  return treconField;
}


tmp<volScalarField> reconstructMag(const tmp<surfaceScalarField>& tssf)
{
  tmp<volScalarField> tvf
  {
    fvc::reconstructMag(tssf())
  };
  tssf.clear();
  return tvf;
}

}  // namespace fvc
}  // namespace mousse
