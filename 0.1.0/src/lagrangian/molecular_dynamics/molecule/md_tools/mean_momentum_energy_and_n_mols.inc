// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

vector singleStepTotalLinearMomentum{vector::zero};
vector singleStepTotalAngularMomentum{vector::zero};
scalar singleStepMaxVelocityMag = 0.0;
scalar singleStepTotalMass = 0.0;
scalar singleStepTotalLinearKE = 0.0;
scalar singleStepTotalAngularKE = 0.0;
scalar singleStepTotalPE = 0.0;
scalar singleStepTotalrDotf = 0.0;
//vector singleStepCentreOfMass(vector::zero);
label singleStepNMols = molecules.size();
label singleStepDOFs = 0;
{
  FOR_ALL_CONST_ITER(IDLList<molecule>, molecules, mol)
  {
    const label molId = mol().id();
    scalar molMass(molecules.constProps(molId).mass());
    singleStepTotalMass += molMass;
    //singleStepCentreOfMass += mol().position()*molMass;
  }
  // if (singleStepNMols)
  // {
  //     singleStepCentreOfMass /= singleStepTotalMass;
  // }
  FOR_ALL_CONST_ITER(IDLList<molecule>, molecules, mol)
  {
    const label molId = mol().id();
    const molecule::constantProperties cP{molecules.constProps(molId)};
    scalar molMass{cP.mass()};
    const diagTensor& molMoI = cP.momentOfInertia();
    const vector& molV = mol().v();
    const vector& molOmega = inv(molMoI) & mol().pi();
    vector molPiGlobal = mol().Q() & mol().pi();
    singleStepTotalLinearMomentum += molV * molMass;
    singleStepTotalAngularMomentum += molPiGlobal;
    //+((mol().position() - singleStepCentreOfMass) ^ (molV * molMass));
    if (mag(molV) > singleStepMaxVelocityMag)
    {
      singleStepMaxVelocityMag = mag(molV);
    }
    singleStepTotalLinearKE += 0.5*molMass*magSqr(molV);
    singleStepTotalAngularKE += 0.5*(molOmega & molMoI & molOmega);
    singleStepTotalPE += mol().potentialEnergy();
    singleStepTotalrDotf += tr(mol().rf());
    singleStepDOFs += cP.degreesOfFreedom();
  }
}
if (Pstream::parRun())
{
  reduce(singleStepTotalLinearMomentum, sumOp<vector>());
  reduce(singleStepTotalAngularMomentum, sumOp<vector>());
  reduce(singleStepMaxVelocityMag, maxOp<scalar>());
  reduce(singleStepTotalMass, sumOp<scalar>());
  reduce(singleStepTotalLinearKE, sumOp<scalar>());
  reduce(singleStepTotalAngularKE, sumOp<scalar>());
  reduce(singleStepTotalPE, sumOp<scalar>());
  reduce(singleStepTotalrDotf, sumOp<scalar>());
  reduce(singleStepNMols, sumOp<label>());
  reduce(singleStepDOFs, sumOp<label>());
}
if (singleStepNMols)
{
  Info<< "Number of molecules in system = "
    << singleStepNMols << nl
    << "Overall number density = "
    << singleStepNMols/meshVolume << nl
    << "Overall mass density = "
    << singleStepTotalMass/meshVolume << nl
    << "Average linear momentum per molecule = "
    << singleStepTotalLinearMomentum/singleStepNMols << ' '
    << mag(singleStepTotalLinearMomentum)/singleStepNMols << nl
    << "Average angular momentum per molecule = "
    << singleStepTotalAngularMomentum << ' '
    << mag(singleStepTotalAngularMomentum)/singleStepNMols << nl
    << "Maximum |velocity| = "
    << singleStepMaxVelocityMag << nl
    << "Average linear KE per molecule = "
    << singleStepTotalLinearKE/singleStepNMols << nl
    << "Average angular KE per molecule = "
    << singleStepTotalAngularKE/singleStepNMols << nl
    << "Average PE per molecule = "
    << singleStepTotalPE/singleStepNMols << nl
    << "Average TE per molecule = "
    <<
    (
      singleStepTotalLinearKE
     + singleStepTotalAngularKE
     + singleStepTotalPE
    )
    /singleStepNMols
    << endl;
    // Info<< singleStepNMols << " "
    //     << singleStepTotalMomentum/singleStepTotalMass << " "
    //     << singleStepMaxVelocityMag << " "
    //     << singleStepTotalKE/singleStepNMols << " "
    //     << singleStepTotalPE/singleStepNMols << " "
    //     << (singleStepTotalKE + singleStepTotalPE)
    //        /singleStepNMols << endl;
}
else
{
  Info<< "No molecules in system" << endl;
}
