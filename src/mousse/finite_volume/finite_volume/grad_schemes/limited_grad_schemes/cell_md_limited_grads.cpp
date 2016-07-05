// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_md_limited_grad.hpp"
#include "gauss_grad.hpp"
#include "fv_mesh.hpp"
#include "vol_mesh.hpp"
#include "surface_mesh.hpp"
#include "vol_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "fvs_patch_field.hpp"
#include "fv.hpp"


MAKE_FV_GRAD_SCHEME(cellMDLimitedGrad)

template<>
mousse::tmp<mousse::volVectorField>
mousse::fv::cellMDLimitedGrad<mousse::scalar>::calcGrad
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
  scalarField maxVsf{vsf.internalField()};
  scalarField minVsf{vsf.internalField()};
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    scalar vsfOwn = vsf[own];
    scalar vsfNei = vsf[nei];
    maxVsf[own] = max(maxVsf[own], vsfNei);
    minVsf[own] = min(minVsf[own], vsfNei);
    maxVsf[nei] = max(maxVsf[nei], vsfOwn);
    minVsf[nei] = min(minVsf[nei], vsfOwn);
  }
  const volScalarField::GeometricBoundaryField& bsf = vsf.boundaryField();
  FOR_ALL(bsf, patchi) {
    const fvPatchScalarField& psf = bsf[patchi];
    const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
    if (psf.coupled()) {
      const scalarField psfNei{psf.patchNeighbourField()};
      FOR_ALL(pOwner, pFacei) {
        label own = pOwner[pFacei];
        scalar vsfNei = psfNei[pFacei];
        maxVsf[own] = max(maxVsf[own], vsfNei);
        minVsf[own] = min(minVsf[own], vsfNei);
      }
    } else {
      FOR_ALL(pOwner, pFacei) {
        label own = pOwner[pFacei];
        scalar vsfNei = psf[pFacei];
        maxVsf[own] = max(maxVsf[own], vsfNei);
        minVsf[own] = min(minVsf[own], vsfNei);
      }
    }
  }
  maxVsf -= vsf;
  minVsf -= vsf;
  if (k_ < 1.0) {
    const scalarField maxMinVsf{(1.0/k_ - 1.0)*(maxVsf - minVsf)};
    maxVsf += maxMinVsf;
    minVsf -= maxMinVsf;
    //maxVsf *= 1.0/k_;
    //minVsf *= 1.0/k_;
  }
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    // owner side
    limitFace
    (
      g[own],
      maxVsf[own],
      minVsf[own],
      Cf[facei] - C[own]
    );
    // neighbour side
    limitFace
    (
      g[nei],
      maxVsf[nei],
      minVsf[nei],
      Cf[facei] - C[nei]
    );
  }
  FOR_ALL(bsf, patchi) {
    const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
    const vectorField& pCf = Cf.boundaryField()[patchi];
    FOR_ALL(pOwner, pFacei) {
      label own = pOwner[pFacei];
      limitFace
      (
        g[own],
        maxVsf[own],
        minVsf[own],
        pCf[pFacei] - C[own]
      );
    }
  }
  g.correctBoundaryConditions();
  gaussGrad<scalar>::correctBoundaryConditions(vsf, g);
  return tGrad;
}


template<>
mousse::tmp<mousse::volTensorField>
mousse::fv::cellMDLimitedGrad<mousse::vector>::calcGrad
(
  const volVectorField& vsf,
  const word& name
) const
{
  const fvMesh& mesh = vsf.mesh();
  tmp<volTensorField> tGrad = basicGradScheme_().calcGrad(vsf, name);
  if (k_ < SMALL) {
    return tGrad;
  }
  volTensorField& g = tGrad();
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  const volVectorField& C = mesh.C();
  const surfaceVectorField& Cf = mesh.Cf();
  vectorField maxVsf{vsf.internalField()};
  vectorField minVsf{vsf.internalField()};
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    const vector& vsfOwn = vsf[own];
    const vector& vsfNei = vsf[nei];
    maxVsf[own] = max(maxVsf[own], vsfNei);
    minVsf[own] = min(minVsf[own], vsfNei);
    maxVsf[nei] = max(maxVsf[nei], vsfOwn);
    minVsf[nei] = min(minVsf[nei], vsfOwn);
  }
  const volVectorField::GeometricBoundaryField& bsf = vsf.boundaryField();
  FOR_ALL(bsf, patchi) {
    const fvPatchVectorField& psf = bsf[patchi];
    const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
    if (psf.coupled()) {
      const vectorField psfNei{psf.patchNeighbourField()};
      FOR_ALL(pOwner, pFacei) {
        label own = pOwner[pFacei];
        const vector& vsfNei = psfNei[pFacei];
        maxVsf[own] = max(maxVsf[own], vsfNei);
        minVsf[own] = min(minVsf[own], vsfNei);
      }
    } else {
      FOR_ALL(pOwner, pFacei) {
        label own = pOwner[pFacei];
        const vector& vsfNei = psf[pFacei];
        maxVsf[own] = max(maxVsf[own], vsfNei);
        minVsf[own] = min(minVsf[own], vsfNei);
      }
    }
  }
  maxVsf -= vsf;
  minVsf -= vsf;
  if (k_ < 1.0) {
    const vectorField maxMinVsf{(1.0/k_ - 1.0)*(maxVsf - minVsf)};
    maxVsf += maxMinVsf;
    minVsf -= maxMinVsf;
    //maxVsf *= 1.0/k_;
    //minVsf *= 1.0/k_;
  }
  FOR_ALL(owner, facei) {
    label own = owner[facei];
    label nei = neighbour[facei];
    // owner side
    limitFace
    (
      g[own],
      maxVsf[own],
      minVsf[own],
      Cf[facei] - C[own]
    );
    // neighbour side
    limitFace
    (
      g[nei],
      maxVsf[nei],
      minVsf[nei],
      Cf[facei] - C[nei]
    );
  }
  FOR_ALL(bsf, patchi) {
    const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
    const vectorField& pCf = Cf.boundaryField()[patchi];
    FOR_ALL(pOwner, pFacei) {
      label own = pOwner[pFacei];
      limitFace
      (
        g[own],
        maxVsf[own],
        minVsf[own],
        pCf[pFacei] - C[own]
      );
    }
  }
  g.correctBoundaryConditions();
  gaussGrad<vector>::correctBoundaryConditions(vsf, g);
  return tGrad;
}

