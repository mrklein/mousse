// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

{
  dimensionedScalar totalMass = fvc::domainIntegrate(rho);
  scalar sumLocalContErr =
    (fvc::domainIntegrate(mag(rho - thermo.rho()))/totalMass).value();
  scalar globalContErr =
    (fvc::domainIntegrate(rho - thermo.rho())/totalMass).value();
  cumulativeContErr += globalContErr;
  Info<< "time step continuity errors : sum local = " << sumLocalContErr
    << ", global = " << globalContErr
    << ", cumulative = " << cumulativeContErr
    << endl;
}
