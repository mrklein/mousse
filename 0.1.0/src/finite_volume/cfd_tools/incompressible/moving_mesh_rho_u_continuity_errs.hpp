// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (mesh.moving())
{
  scalar sumLocalContErr = runTime.deltaTValue()*
    mag(fvc::div(phi + fvc::meshPhi(rho, U)))()
   .weightedAverage(mesh.V()).value();
  scalar globalContErr = runTime.deltaTValue()*
    fvc::div(phi + fvc::meshPhi(rho, U))()
   .weightedAverage(mesh.V()).value();
  cumulativeContErr += globalContErr;
  Info<< "time step continuity errors : sum local = " << sumLocalContErr
    << ", global = " << globalContErr
    << ", cumulative = " << cumulativeContErr
    << endl;
}
else
{
#   include "continuity_errs.hpp"
}
