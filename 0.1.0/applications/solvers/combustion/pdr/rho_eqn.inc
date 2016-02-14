// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

{
  solve(betav*fvm::ddt(rho) + fvc::div(phi));
}
