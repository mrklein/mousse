#ifndef SOLVERS_HEAT_TRANSFER_CHT_MULTI_REGION_FLUID_COMPRESSIBLE_COURANT_NO_HPP_
#define SOLVERS_HEAT_TRANSFER_CHT_MULTI_REGION_FLUID_COMPRESSIBLE_COURANT_NO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "fv_mesh.hpp"
namespace mousse
{
  scalar compressibleCourantNo
  (
    const fvMesh& mesh,
    const Time& runTime,
    const volScalarField& rho,
    const surfaceScalarField& phi
  );
}
#endif
