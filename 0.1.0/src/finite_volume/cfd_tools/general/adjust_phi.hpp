#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_ADJUST_PHI_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_ADJUST_PHI_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   adjust_phi.cpp
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
//- Adjust the balance of fluxes to obey continuity.
//  For cases which do not have a pressure boundary.
//  Return true if the domain is closed.
bool adjustPhi
(
  surfaceScalarField& phi,
  const volVectorField& U,
  volScalarField& p
);
}  // namespace mousse
#endif
