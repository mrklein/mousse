// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CorrectPhi
// Description
//   Flux correction functions to ensure continuity.
//   Required during start-up, restart, mesh-motion etc. when non-conservative
//   fluxes may adversely affect the prediction-part of the solution algorithm
//   (the part before the first pressure solution which would ensure continuity).
//   This is particularly important for VoF and other multi-phase solver in
//   which non-conservative fluxes cause unboundedness of the phase-fraction.
// SourceFiles
//   correct_phi.cpp
#ifndef correct_phi_hpp_
#define correct_phi_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
  class pimpleControl;
  //- If the mesh is moving correct the velocity BCs on the moving walls to
  //  ensure the corrected fluxes and velocity are consistent
  void correctUphiBCs
  (
    volVectorField& U,
    surfaceScalarField& phi
  );
  //- If the mesh is moving correct the velocity BCs on the moving walls to
  //  ensure the corrected fluxes and velocity are consistent
  void correctUphiBCs
  (
    const volScalarField& rho,
    volVectorField& U,
    surfaceScalarField& phi
  );
  template<class RAUfType, class DivUType>
  void CorrectPhi
  (
    volVectorField& U,
    surfaceScalarField& phi,
    const volScalarField& p,
    const RAUfType& rAUf,
    const DivUType& divU,
    pimpleControl& pimple
  );
  template<class RAUfType, class DivRhoUType>
  void CorrectPhi
  (
    volVectorField& U,
    surfaceScalarField& phi,
    const volScalarField& p,
    const volScalarField& rho,
    const volScalarField& psi,
    const RAUfType& rAUf,
    const DivRhoUType& divRhoU,
    pimpleControl& pimple
  );
}
#ifdef NoRepository
#   include "correct_phi.cpp"
#endif
#endif
