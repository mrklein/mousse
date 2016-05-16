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
  scalar maxDeltaTFluid = maxCo/(CoNum + SMALL);
  scalar maxDeltaTSolid = maxDi/(DiNum + SMALL);
  scalar deltaTFluid =
    min
    (
      min(maxDeltaTFluid, 1.0 + 0.1*maxDeltaTFluid),
      1.2
    );
  runTime.setDeltaT
  (
    min
    (
      min(deltaTFluid, maxDeltaTSolid)*runTime.deltaT().value(),
      maxDeltaT
    )
  );
  Info << "deltaT = " <<  runTime.deltaT().value() << endl;
}
