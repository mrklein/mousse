// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

accumulatedTotalLinearMomentum += singleStepTotalLinearMomentum;
accumulatedTotalMass += singleStepTotalMass;
accumulatedTotalLinearKE += singleStepTotalLinearKE;
accumulatedTotalAngularKE += singleStepTotalAngularKE;
accumulatedTotalPE += singleStepTotalPE;
accumulatedTotalrDotfSum += singleStepTotalrDotf;
accumulatedNMols += singleStepNMols;
accumulatedDOFs += singleStepDOFs;
if (runTime.outputTime())
{
  if (accumulatedNMols)
  {
    Info<< "calculating averages" << endl;
    averageTemperature =
    (
      2.0/(physicoChemical::k.value()*accumulatedDOFs)
      *
      (
        accumulatedTotalLinearKE + accumulatedTotalAngularKE
        -
        0.5*magSqr(accumulatedTotalLinearMomentum)/accumulatedTotalMass
      )
    );
    averagePressure =
    (
      (
        (accumulatedNMols/nAveragingSteps)
       *physicoChemical::k.value()*averageTemperature
       + accumulatedTotalrDotfSum/(6.0*nAveragingSteps)
      )
      /
      meshVolume
    );
    Info<< "----------------------------------------" << nl
      << "Averaged properties" << nl
      << "Average |velocity| = "
      << mag(accumulatedTotalLinearMomentum)/accumulatedTotalMass << nl
      << "Average temperature = " << averageTemperature << nl
      << "Average pressure = " << averagePressure << nl
      << "----------------------------------------" << endl;
  }
  else
  {
    Info<< "Not averaging temperature and pressure: "
      << "no molecules in system" << endl;
  }
  accumulatedTotalLinearMomentum = vector::zero;
  accumulatedTotalMass = 0.0;
  accumulatedTotalLinearKE = 0.0;
  accumulatedTotalAngularKE = 0.0;
  accumulatedTotalPE = 0.0;
  accumulatedTotalrDotfSum = 0.0;
  accumulatedNMols = 0;
  accumulatedDOFs = 0;
}
