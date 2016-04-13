// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_md_limited_grad.hpp"
#include "cell_md_limited_grad.hpp"
#include "gauss_grad.hpp"
#include "fv_mesh.hpp"
#include "vol_mesh.hpp"
#include "surface_mesh.hpp"
#include "vol_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "fvs_patch_field.hpp"
#include "fv.hpp"


MAKE_FV_GRAD_SCHEME(faceMDLimitedGrad)

template<>
mousse::tmp<mousse::volVectorField>
mousse::fv::faceMDLimitedGrad<mousse::scalar>::calcGrad
(
  const volScalarField& vsf,
  const word& name
) const
{
  const fvMesh& mesh = vsf.mesh();
  tmp<volVectorField> tGrad = basicGradScheme_().calcGrad(vsf, name);
  if (k_ < SMALL) {
    return tGrad;
  }
  volVectorField& g = tGrad();
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  const volVectorField& C = mesh.C();
  const surfaceVectorField& Cf = mesh.Cf();
  scalar rk = (1.0/k_ - 1.0);
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    scalar vsfOwn = vsf[own];
    scalar vsfNei = vsf[nei];
    scalar maxFace = max(vsfOwn, vsfNei);
    scalar minFace = min(vsfOwn, vsfNei);
    if (k_ < 1.0) {
      scalar maxMinFace = rk*(maxFace - minFace);
      maxFace += maxMinFace;
      minFace -= maxMinFace;
    }
    // owner side
    cellMDLimitedGrad<scalar>::limitFace
    (
      g[own],
      maxFace - vsfOwn,
      minFace - vsfOwn,
      Cf[facei] - C[own]
    );
    // neighbour side
    cellMDLimitedGrad<scalar>::limitFace
    (
      g[nei],
      maxFace - vsfNei,
      minFace - vsfNei,
      Cf[facei] - C[nei]
    );
  }
  const volScalarField::GeometricBoundaryField& bsf = vsf.boundaryField();
  FOR_ALL(bsf, patchi) {
    const fvPatchScalarField& psf = bsf[patchi];
    const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
    const vectorField& pCf = Cf.boundaryField()[patchi];
    if (psf.coupled()) {
      const scalarField psfNei{psf.patchNeighbourField()};
      FOR_ALL(pOwner, pFacei) {
        label own = pOwner[pFacei];
        scalar vsfOwn = vsf[own];
        scalar vsfNei = psfNei[pFacei];
        scalar maxFace = max(vsfOwn, vsfNei);
        scalar minFace = min(vsfOwn, vsfNei);
        if (k_ < 1.0) {
          scalar maxMinFace = rk*(maxFace - minFace);
          maxFace += maxMinFace;
          minFace -= maxMinFace;
        }
        cellMDLimitedGrad<scalar>::limitFace
        (
          g[own],
          maxFace - vsfOwn,
          minFace - vsfOwn,
          pCf[pFacei] - C[own]
        );
      }
    } else if (psf.fixesValue()) {
      FOR_ALL(pOwner, pFacei) {
        label own = pOwner[pFacei];
        scalar vsfOwn = vsf[own];
        scalar vsfNei = psf[pFacei];
        scalar maxFace = max(vsfOwn, vsfNei);
        scalar minFace = min(vsfOwn, vsfNei);
        if (k_ < 1.0) {
          scalar maxMinFace = rk*(maxFace - minFace);
          maxFace += maxMinFace;
          minFace -= maxMinFace;
        }
        cellMDLimitedGrad<scalar>::limitFace
        (
          g[own],
          maxFace - vsfOwn,
          minFace - vsfOwn,
          pCf[pFacei] - C[own]
        );
      }
    }
  }
  g.correctBoundaryConditions();
  gaussGrad<scalar>::correctBoundaryConditions(vsf, g);
  return tGrad;
}


template<>
mousse::tmp<mousse::volTensorField>
mousse::fv::faceMDLimitedGrad<mousse::vector>::calcGrad
(
  const volVectorField& vvf,
  const word& name
) const
{
  const fvMesh& mesh = vvf.mesh();
  tmp<volTensorField> tGrad = basicGradScheme_().calcGrad(vvf, name);
  if (k_ < SMALL) {
    return tGrad;
  }
  volTensorField& g = tGrad();
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  const volVectorField& C = mesh.C();
  const surfaceVectorField& Cf = mesh.Cf();
  scalar rk = (1.0/k_ - 1.0);
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    vector vvfOwn = vvf[own];
    vector vvfNei = vvf[nei];
    vector maxFace = max(vvfOwn, vvfNei);
    vector minFace = min(vvfOwn, vvfNei);
    if (k_ < 1.0) {
      vector maxMinFace = rk*(maxFace - minFace);
      maxFace += maxMinFace;
      minFace -= maxMinFace;
    }
    // owner side
    cellMDLimitedGrad<vector>::limitFace
    (
      g[own],
      maxFace - vvfOwn,
      minFace - vvfOwn,
      Cf[facei] - C[own]
    );
    // neighbour side
    cellMDLimitedGrad<vector>::limitFace
    (
      g[nei],
      maxFace - vvfNei,
      minFace - vvfNei,
      Cf[facei] - C[nei]
    );
  }
  const volVectorField::GeometricBoundaryField& bvf = vvf.boundaryField();
  FOR_ALL(bvf, patchi) {
    const fvPatchVectorField& psf = bvf[patchi];
    const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
    const vectorField& pCf = Cf.boundaryField()[patchi];
    if (psf.coupled()) {
      const vectorField psfNei{psf.patchNeighbourField()};
      FOR_ALL(pOwner, pFacei) {
        label own = pOwner[pFacei];
        vector vvfOwn = vvf[own];
        vector vvfNei = psfNei[pFacei];
        vector maxFace = max(vvfOwn, vvfNei);
        vector minFace = min(vvfOwn, vvfNei);
        if (k_ < 1.0) {
          vector maxMinFace = rk*(maxFace - minFace);
          maxFace += maxMinFace;
          minFace -= maxMinFace;
        }
        cellMDLimitedGrad<vector>::limitFace
        (
          g[own],
          maxFace - vvfOwn, minFace - vvfOwn,
          pCf[pFacei] - C[own]
        );
      }
    } else if (psf.fixesValue()) {
      FOR_ALL(pOwner, pFacei) {
        label own = pOwner[pFacei];
        vector vvfOwn = vvf[own];
        vector vvfNei = psf[pFacei];
        vector maxFace = max(vvfOwn, vvfNei);
        vector minFace = min(vvfOwn, vvfNei);
        if (k_ < 1.0) {
          vector maxMinFace = rk*(maxFace - minFace);
          maxFace += maxMinFace;
          minFace -= maxMinFace;
        }
        cellMDLimitedGrad<vector>::limitFace
        (
          g[own],
          maxFace - vvfOwn,
          minFace - vvfOwn,
          pCf[pFacei] - C[own]
        );
      }
    }
  }
  g.correctBoundaryConditions();
  gaussGrad<vector>::correctBoundaryConditions(vvf, g);
  return tGrad;
}

