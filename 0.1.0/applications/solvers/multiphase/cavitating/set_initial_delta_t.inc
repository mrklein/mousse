// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (adjustTimeStep) {
  if (CoNum > SMALL) {
    scalar maxDeltaTFact =
      min(maxCo/(CoNum + SMALL), maxAcousticCo/(acousticCoNum + SMALL));
    runTime.setDeltaT
    (
      min
      (
        maxDeltaTFact*runTime.deltaTValue(),
        maxDeltaT
      )
    );
  }
}
