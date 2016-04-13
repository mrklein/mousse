// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

{
  volScalarField contErr = fvc::div(phi + fvc::meshPhi(U));
  scalar sumLocalContErr =
    runTime.deltaTValue()*mag(contErr)().weightedAverage(mesh.V()).value();
  scalar globalContErr =
    runTime.deltaTValue()*contErr.weightedAverage(mesh.V()).value();
  cumulativeContErr += globalContErr;
  Info << "time step continuity errors : sum local = " << sumLocalContErr
    << ", global = " << globalContErr
    << ", cumulative = " << cumulativeContErr
    << endl;
}
