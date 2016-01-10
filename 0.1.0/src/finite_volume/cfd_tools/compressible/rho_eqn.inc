// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

{
  fvScalarMatrix rhoEqn
  (
    fvm::ddt(rho)
   + fvc::div(phi)
   ==
    fvOptions(rho)
  );

  fvOptions.constrain(rhoEqn);

  rhoEqn.solve();

  fvOptions.correct(rho);
}
