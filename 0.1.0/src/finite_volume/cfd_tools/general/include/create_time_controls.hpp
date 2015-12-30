// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

bool adjustTimeStep =
  runTime.controlDict().lookupOrDefault("adjustTimeStep", false);
scalar maxCo =
  runTime.controlDict().lookupOrDefault<scalar>("maxCo", 1.0);
scalar maxDeltaT =
  runTime.controlDict().lookupOrDefault<scalar>("maxDeltaT", GREAT);
