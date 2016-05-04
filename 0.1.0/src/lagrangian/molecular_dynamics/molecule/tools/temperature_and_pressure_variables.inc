// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

vector accumulatedTotalLinearMomentum(vector::zero);
scalar accumulatedTotalMass = 0.0;
scalar accumulatedTotalAngularKE = 0.0;
scalar accumulatedTotalLinearKE = 0.0;
scalar accumulatedTotalPE = 0.0;
scalar accumulatedTotalrDotfSum = 0.0;
label accumulatedNMols = 0;
label accumulatedDOFs = 0;
scalar averageTemperature = 0.0;
scalar averagePressure = 0.0;
const scalarField& cellVols = mesh.cellVolumes();
scalar meshVolume = sum(cellVols);
if (Pstream::parRun()) {
  reduce(meshVolume, sumOp<scalar>());
}
