// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (adjustTimeStep) {
  if ((runTime.timeIndex() == 0) && (CoNum > SMALL)) {
    runTime.setDeltaT
    (
      min
      (
        maxCo*runTime.deltaTValue()/CoNum,
        min(runTime.deltaTValue(), maxDeltaT)
      )
    );
  }
}
