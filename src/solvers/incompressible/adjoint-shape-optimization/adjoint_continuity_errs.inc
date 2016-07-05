// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

{
  scalar sumLocalContErr = runTime.deltaTValue()*
    mag(fvc::div(phia))().weightedAverage(mesh.V()).value();
  scalar globalContErr = runTime.deltaTValue()*
    fvc::div(phia)().weightedAverage(mesh.V()).value();
  cumulativeAdjointContErr += globalContErr;
  Info << "Adjoint continuity errors : sum local = " << sumLocalContErr
    << ", global = " << globalContErr
    << ", cumulative = " << cumulativeAdjointContErr
    << endl;
}
