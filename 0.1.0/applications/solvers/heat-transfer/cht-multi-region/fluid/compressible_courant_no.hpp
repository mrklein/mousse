// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef compressible_courant_no_hpp_
#define compressible_courant_no_hpp_

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
