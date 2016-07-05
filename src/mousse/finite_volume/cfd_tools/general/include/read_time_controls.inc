// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

adjustTimeStep =
  runTime.controlDict().lookupOrDefault("adjustTimeStep", false);
maxCo =
  runTime.controlDict().lookupOrDefault<scalar>("maxCo", 1.0);
maxDeltaT =
  runTime.controlDict().lookupOrDefault<scalar>("maxDeltaT", GREAT);
