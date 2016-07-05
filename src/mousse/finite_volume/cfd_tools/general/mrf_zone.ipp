// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mrf_zone.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_matrices.hpp"


// Private Member Functions 
template<class RhoFieldType>
void mousse::MRFZone::makeRelativeRhoFlux
(
  const RhoFieldType& rho,
  surfaceScalarField& phi
) const
{
  const surfaceVectorField& Cf = mesh_.Cf();
  const surfaceVectorField& Sf = mesh_.Sf();
  const vector Omega = omega_->value(mesh_.time().timeOutputValue())*axis_;
  const vectorField& Cfi = Cf.internalField();
  const vectorField& Sfi = Sf.internalField();
  scalarField& phii = phi.internalField();
  // Internal faces
  FOR_ALL(internalFaces_, i) {
    label facei = internalFaces_[i];
    phii[facei] -= rho[facei]*(Omega ^ (Cfi[facei] - origin_)) & Sfi[facei];
  }
  makeRelativeRhoFlux(rho.boundaryField(), phi.boundaryField());
}


template<class RhoFieldType>
void mousse::MRFZone::makeRelativeRhoFlux
(
  const RhoFieldType& rho,
  FieldField<fvsPatchField, scalar>& phi
) const
{
  const surfaceVectorField& Cf = mesh_.Cf();
  const surfaceVectorField& Sf = mesh_.Sf();
  const vector Omega = omega_->value(mesh_.time().timeOutputValue())*axis_;
  // Included patches
  FOR_ALL(includedFaces_, patchi) {
    FOR_ALL(includedFaces_[patchi], i) {
      label patchFacei = includedFaces_[patchi][i];
      phi[patchi][patchFacei] = 0.0;
    }
  }
  // Excluded patches
  FOR_ALL(excludedFaces_, patchi) {
    FOR_ALL(excludedFaces_[patchi], i) {
      label patchFacei = excludedFaces_[patchi][i];
      phi[patchi][patchFacei] -= rho[patchi][patchFacei]
        *(Omega ^ (Cf.boundaryField()[patchi][patchFacei] - origin_))
        & Sf.boundaryField()[patchi][patchFacei];
    }
  }
}


template<class RhoFieldType>
void mousse::MRFZone::makeAbsoluteRhoFlux
(
  const RhoFieldType& rho,
  surfaceScalarField& phi
) const
{
  const surfaceVectorField& Cf = mesh_.Cf();
  const surfaceVectorField& Sf = mesh_.Sf();
  const vector Omega = omega_->value(mesh_.time().timeOutputValue())*axis_;
  const vectorField& Cfi = Cf.internalField();
  const vectorField& Sfi = Sf.internalField();
  scalarField& phii = phi.internalField();
  // Internal faces
  FOR_ALL(internalFaces_, i) {
    label facei = internalFaces_[i];
    phii[facei] += rho[facei]*(Omega ^ (Cfi[facei] - origin_)) & Sfi[facei];
  }
  // Included patches
  FOR_ALL(includedFaces_, patchi) {
    FOR_ALL(includedFaces_[patchi], i) {
      label patchFacei = includedFaces_[patchi][i];
      phi.boundaryField()[patchi][patchFacei] +=
        rho.boundaryField()[patchi][patchFacei]
        *(Omega ^ (Cf.boundaryField()[patchi][patchFacei] - origin_))
        & Sf.boundaryField()[patchi][patchFacei];
    }
  }
  // Excluded patches
  FOR_ALL(excludedFaces_, patchi) {
    FOR_ALL(excludedFaces_[patchi], i) {
      label patchFacei = excludedFaces_[patchi][i];
      phi.boundaryField()[patchi][patchFacei] +=
        rho.boundaryField()[patchi][patchFacei]
        *(Omega ^ (Cf.boundaryField()[patchi][patchFacei] - origin_))
        & Sf.boundaryField()[patchi][patchFacei];
    }
  }
}

