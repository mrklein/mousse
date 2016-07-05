// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

{
  volScalarField contErr{fvc::ddt(alphac) + fvc::div(alphacf*phic)};
  scalar sumLocalContErr = runTime.deltaTValue()*
    mag(contErr)().weightedAverage(mesh.V()).value();
  scalar globalContErr = runTime.deltaTValue()*
    contErr.weightedAverage(mesh.V()).value();
  cumulativeContErr += globalContErr;
  Info << "time step continuity errors : sum local = " << sumLocalContErr
    << ", global = " << globalContErr
    << ", cumulative = " << cumulativeContErr
    << endl;
}
