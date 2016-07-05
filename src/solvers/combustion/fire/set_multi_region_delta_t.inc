// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (adjustTimeStep) {
  if (CoNum == -GREAT) {
    CoNum = SMALL;
  }
  if (DiNum == -GREAT) {
    DiNum = SMALL;
  }
  const scalar TFactorFluid = maxCo/(CoNum + SMALL);
  const scalar TFactorSolid = maxDi/(DiNum + SMALL);
  const scalar TFactorFilm = maxCo/(surfaceFilm.CourantNumber() + SMALL);
  const scalar dt0 = runTime.deltaTValue();
  runTime.setDeltaT
  (
    min
    (
      dt0*min(min(TFactorFluid, min(TFactorFilm, TFactorSolid)), 1.2),
      maxDeltaT
    )
  );
}

