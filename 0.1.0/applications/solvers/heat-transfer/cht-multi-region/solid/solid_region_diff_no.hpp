#ifndef SOLVERS_HEAT_TRANSFER_CHT_MULTI_REGION_SOLID_SOLID_REGION_DIFF_NO_HPP_
#define SOLVERS_HEAT_TRANSFER_CHT_MULTI_REGION_SOLID_SOLID_REGION_DIFF_NO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
namespace mousse
{
  scalar solidRegionDiffNo
  (
    const fvMesh& mesh,
    const Time& runTime,
    const volScalarField& Cprho,
    const volScalarField& kappa
  );
}
#endif
