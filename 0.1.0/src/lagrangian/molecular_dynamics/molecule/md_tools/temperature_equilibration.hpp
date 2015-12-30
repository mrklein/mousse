// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

if (runTime.outputTime())
{
  molecules.applyConstraintsAndThermostats
  (
    targetTemperature,
    averageTemperature
  );
}
