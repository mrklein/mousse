// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef solid_region_diff_no_hpp_
#define solid_region_diff_no_hpp_
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
