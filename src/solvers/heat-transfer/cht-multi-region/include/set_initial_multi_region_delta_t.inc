// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (adjustTimeStep) {
  if ((runTime.timeIndex() == 0) && ((CoNum > SMALL) || (DiNum > SMALL))) {
    if (CoNum < SMALL) {
      CoNum = SMALL;
    }
    if (DiNum < SMALL) {
      DiNum = SMALL;
    }
    runTime.setDeltaT
    (
      min
      (
        min(maxCo/CoNum, maxDi/DiNum)*runTime.deltaT().value(),
        maxDeltaT
      )
    );
    Info << "deltaT = " <<  runTime.deltaT().value() << endl;
  }
}
