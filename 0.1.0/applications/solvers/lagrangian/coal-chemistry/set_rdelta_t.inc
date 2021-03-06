// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

{
  volScalarField& rDeltaT = trDeltaT();
  const dictionary& pimpleDict = pimple.dict();
  // Maximum flow Courant number
  scalar maxCo{readScalar(pimpleDict.lookup("maxCo"))};
  // Maximum time scale
  scalar maxDeltaT{pimpleDict.lookupOrDefault<scalar>("maxDeltaT", GREAT)};
  // Smoothing parameter (0-1) when smoothing iterations > 0
  scalar rDeltaTSmoothingCoeff
  {
    pimpleDict.lookupOrDefault<scalar>("rDeltaTSmoothingCoeff", 0.1)
  };
  // Damping coefficient (1-0)
  scalar rDeltaTDampingCoeff
  {
    pimpleDict.lookupOrDefault<scalar>("rDeltaTDampingCoeff", 0.2)
  };
  // Maximum change in cell temperature per iteration
  // (relative to previous value)
  scalar alphaTemp{pimpleDict.lookupOrDefault("alphaTemp", 0.05)};
  Info << "Time scales min/max:" << endl;
  // Cache old reciprocal time scale field
  volScalarField rDeltaT0{"rDeltaT0", rDeltaT};
  // Flow time scale
  {
    rDeltaT.dimensionedInternalField() =
    (
      fvc::surfaceSum(mag(phi))().dimensionedInternalField()
      /((2*maxCo)*mesh.V()*rho.dimensionedInternalField())
    );
    // Limit the largest time scale
    rDeltaT.max(1/maxDeltaT);
    Info << "    Flow        = " << gMin(1/rDeltaT.internalField()) << ", "
      << gMax(1/rDeltaT.internalField()) << endl;
  }
  // Reaction source time scale
  {
    volScalarField::DimensionedInternalField rDeltaTT
    {
      mag
      (
        (coalParcels.hsTrans() + limestoneParcels.hsTrans())
          /(mesh.V()*runTime.deltaT()) + combustion->Sh()()
      )/(alphaTemp*rho.dimensionedInternalField()
       *thermo.Cp()().dimensionedInternalField()*T.dimensionedInternalField())
    };
    Info << "    Temperature = " << gMin(1/(rDeltaTT.field() + VSMALL)) << ", "
      << gMax(1/(rDeltaTT.field() + VSMALL)) << endl;
    rDeltaT.dimensionedInternalField() =
      max
      (
        rDeltaT.dimensionedInternalField(),
        rDeltaTT
      );
  }
  // Update tho boundary values of the reciprocal time-step
  rDeltaT.correctBoundaryConditions();
  // Spatially smooth the time scale field
  if (rDeltaTSmoothingCoeff < 1.0) {
    fvc::smooth(rDeltaT, rDeltaTSmoothingCoeff);
  }
  // Limit rate of change of time scale
  // - reduce as much as required
  // - only increase at a fraction of old time scale
  if (rDeltaTDampingCoeff < 1.0
      && runTime.timeIndex() > runTime.startTimeIndex() + 1) {
    rDeltaT =
      max
      (
        rDeltaT,
        (scalar(1.0) - rDeltaTDampingCoeff)*rDeltaT0
      );
  }
  Info << "    Overall     = " << gMin(1/rDeltaT.internalField())
    << ", " << gMax(1/rDeltaT.internalField()) << endl;
}
