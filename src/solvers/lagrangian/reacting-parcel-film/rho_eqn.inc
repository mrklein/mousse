// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

{
  fvScalarMatrix rhoEqn
  {
    fvm::ddt(rho)
  + fvc::div(phi)
  ==
    parcels.Srho(rho)
  + surfaceFilm.Srho()
  + fvOptions(rho)
  };
  rhoEqn.solve();
  fvOptions.correct(rho);
}
