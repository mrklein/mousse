// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (adjustTimeStep)
{
  scalar maxDeltaTFact =
    min(maxCo/(CoNum + SMALL), maxAcousticCo/(acousticCoNum + SMALL));
  scalar deltaTFact = min(min(maxDeltaTFact, 1.0 + 0.1*maxDeltaTFact), 1.2);
  runTime.setDeltaT
  (
    min
    (
      deltaTFact*runTime.deltaTValue(),
      maxDeltaT
    )
  );
  Info<< "deltaT = " <<  runTime.deltaTValue() << endl;
}
