// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   adjust_phi.cpp
#ifndef adjust_phi_hpp_
#define adjust_phi_hpp_
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
