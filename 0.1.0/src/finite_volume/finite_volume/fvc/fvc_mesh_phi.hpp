#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_MESH_PHI_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_MESH_PHI_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_mesh_phi.cpp
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "dimensioned_types.hpp"
namespace mousse
{
namespace fvc
{
  tmp<surfaceScalarField> meshPhi
  (
    const volVectorField& U
  );
  tmp<surfaceScalarField> meshPhi
  (
    const dimensionedScalar& rho,
    const volVectorField& U
  );
  tmp<surfaceScalarField> meshPhi
  (
    const volScalarField& rho,
    const volVectorField& U
  );
  //- Make the given flux relative
  void makeRelative
  (
    surfaceScalarField& phi,
    const volVectorField& U
  );
  //- Make the given flux relative
  void makeRelative
  (
    surfaceScalarField& phi,
    const dimensionedScalar& rho,
    const volVectorField& U
  );
  //- Make the given flux relative
  void makeRelative
  (
    surfaceScalarField& phi,
    const volScalarField& rho,
    const volVectorField& U
  );
  //- Make the given flux absolute
  void makeAbsolute
  (
    surfaceScalarField& phi,
    const volVectorField& U
  );
  //- Make the given flux absolute
  void makeAbsolute
  (
    surfaceScalarField& phi,
    const dimensionedScalar& rho,
    const volVectorField& U
  );
  //- Make the given flux absolute
  void makeAbsolute
  (
    surfaceScalarField& phi,
    const volScalarField& rho,
    const volVectorField& U
  );
  //- Return the given absolute flux in relative form
  tmp<surfaceScalarField> relative
  (
    const tmp<surfaceScalarField>& tphi,
    const volVectorField& U
  );
  //- Return the given absolute flux in relative form
  tmp<surfaceScalarField> relative
  (
    const tmp<surfaceScalarField>& tphi,
    const volScalarField& rho,
    const volVectorField& U
  );
  //- Return the given relative flux in absolute form
  tmp<surfaceScalarField> absolute
  (
    const tmp<surfaceScalarField>& tphi,
    const volVectorField& U
  );
  //- Return the given relative flux in absolute form
  tmp<surfaceScalarField> absolute
  (
    const tmp<surfaceScalarField>& tphi,
    const volScalarField& rho,
    const volVectorField& U
  );
}
}  // namespace mousse
#endif
