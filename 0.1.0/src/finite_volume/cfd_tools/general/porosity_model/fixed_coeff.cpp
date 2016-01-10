// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "fixed_coeff.hpp"
#include "fv_matrices.hpp"
// Static Data Members
namespace mousse
{
namespace porosityModels
{
DEFINE_TYPE_NAME_AND_DEBUG(fixedCoeff, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(porosityModel, fixedCoeff, mesh);
}
}
// Private Member Functions 
void mousse::porosityModels::fixedCoeff::apply
(
  scalarField& Udiag,
  vectorField& Usource,
  const scalarField& V,
  const vectorField& U,
  const scalar rho
) const
{
  FOR_ALL(cellZoneIDs_, zoneI)
  {
    const tensorField& alphaZones = alpha_[zoneI];
    const tensorField& betaZones = beta_[zoneI];
    const labelList& cells = mesh_.cellZones()[cellZoneIDs_[zoneI]];
    FOR_ALL(cells, i)
    {
      const label cellI = cells[i];
      const label j = fieldIndex(i);
      const tensor Cd = rho*(alphaZones[j] + betaZones[j]*mag(U[cellI]));
      const scalar isoCd = tr(Cd);
      Udiag[cellI] += V[cellI]*isoCd;
      Usource[cellI] -= V[cellI]*((Cd - I*isoCd) & U[cellI]);
    }
  }
}
void mousse::porosityModels::fixedCoeff::apply
(
  tensorField& AU,
  const vectorField& U,
  const scalar rho
) const
{
  FOR_ALL(cellZoneIDs_, zoneI)
  {
    const tensorField& alphaZones = alpha_[zoneI];
    const tensorField& betaZones = beta_[zoneI];
    const labelList& cells = mesh_.cellZones()[cellZoneIDs_[zoneI]];
    FOR_ALL(cells, i)
    {
      const label cellI = cells[i];
      const label j = fieldIndex(i);
      const tensor alpha = alphaZones[j];
      const tensor beta = betaZones[j];
      AU[cellI] += rho*(alpha + beta*mag(U[cellI]));
    }
  }
}
// Constructors 
mousse::porosityModels::fixedCoeff::fixedCoeff
(
  const word& name,
  const word& modelType,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& cellZoneName
)
:
  porosityModel{name, modelType, mesh, dict, cellZoneName},
  alphaXYZ_{"alpha", dimless/dimTime, coeffs_},
  betaXYZ_{"beta", dimless/dimLength, coeffs_},
  alpha_{cellZoneIDs_.size()},
  beta_{cellZoneIDs_.size()}
{
  adjustNegativeResistance(alphaXYZ_);
  adjustNegativeResistance(betaXYZ_);
  calcTranformModelData();
}
// Destructor 
mousse::porosityModels::fixedCoeff::~fixedCoeff()
{}
// Member Functions 
void mousse::porosityModels::fixedCoeff::calcTranformModelData()
{
  if (coordSys_.R().uniform())
  {
    FOR_ALL(cellZoneIDs_, zoneI)
    {
      alpha_[zoneI].setSize(1);
      beta_[zoneI].setSize(1);
      alpha_[zoneI][0] = tensor::zero;
      alpha_[zoneI][0].xx() = alphaXYZ_.value().x();
      alpha_[zoneI][0].yy() = alphaXYZ_.value().y();
      alpha_[zoneI][0].zz() = alphaXYZ_.value().z();
      alpha_[zoneI][0] = coordSys_.R().transformTensor(alpha_[zoneI][0]);
      beta_[zoneI][0] = tensor::zero;
      beta_[zoneI][0].xx() = betaXYZ_.value().x();
      beta_[zoneI][0].yy() = betaXYZ_.value().y();
      beta_[zoneI][0].zz() = betaXYZ_.value().z();
      beta_[zoneI][0] = coordSys_.R().transformTensor(beta_[zoneI][0]);
    }
  }
  else
  {
    FOR_ALL(cellZoneIDs_, zoneI)
    {
      const labelList& cells = mesh_.cellZones()[cellZoneIDs_[zoneI]];
      alpha_[zoneI].setSize(cells.size());
      beta_[zoneI].setSize(cells.size());
      FOR_ALL(cells, i)
      {
        alpha_[zoneI][i] = tensor::zero;
        alpha_[zoneI][i].xx() = alphaXYZ_.value().x();
        alpha_[zoneI][i].yy() = alphaXYZ_.value().y();
        alpha_[zoneI][i].zz() = alphaXYZ_.value().z();
        beta_[zoneI][i] = tensor::zero;
        beta_[zoneI][i].xx() = betaXYZ_.value().x();
        beta_[zoneI][i].yy() = betaXYZ_.value().y();
        beta_[zoneI][i].zz() = betaXYZ_.value().z();
      }
      const coordinateRotation& R = coordSys_.R(mesh_, cells);
      alpha_[zoneI] = R.transformTensor(alpha_[zoneI], cells);
      beta_[zoneI] = R.transformTensor(beta_[zoneI], cells);
    }
  }
}
void mousse::porosityModels::fixedCoeff::calcForce
(
  const volVectorField& U,
  const volScalarField& /*rho*/,
  const volScalarField& /*mu*/,
  vectorField& force
) const
{
  scalarField Udiag(U.size(), 0.0);
  vectorField Usource(U.size(), vector::zero);
  const scalarField& V = mesh_.V();
  scalar rhoRef = readScalar(coeffs_.lookup("rhoRef"));
  apply(Udiag, Usource, V, U, rhoRef);
  force = Udiag*U - Usource;
}
void mousse::porosityModels::fixedCoeff::correct
(
  fvVectorMatrix& UEqn
) const
{
  const vectorField& U = UEqn.psi();
  const scalarField& V = mesh_.V();
  scalarField& Udiag = UEqn.diag();
  vectorField& Usource = UEqn.source();
  scalar rho = 1.0;
  if (UEqn.dimensions() == dimForce)
  {
    coeffs_.lookup("rhoRef") >> rho;
  }
  apply(Udiag, Usource, V, U, rho);
}
void mousse::porosityModels::fixedCoeff::correct
(
  fvVectorMatrix& UEqn,
  const volScalarField&,
  const volScalarField&
) const
{
  const vectorField& U = UEqn.psi();
  const scalarField& V = mesh_.V();
  scalarField& Udiag = UEqn.diag();
  vectorField& Usource = UEqn.source();
  scalar rho = 1.0;
  if (UEqn.dimensions() == dimForce)
  {
    coeffs_.lookup("rhoRef") >> rho;
  }
  apply(Udiag, Usource, V, U, rho);
}
void mousse::porosityModels::fixedCoeff::correct
(
  const fvVectorMatrix& UEqn,
  volTensorField& AU
) const
{
  const vectorField& U = UEqn.psi();
  scalar rho = 1.0;
  if (UEqn.dimensions() == dimForce)
  {
    coeffs_.lookup("rhoRef") >> rho;
  }
  apply(AU, U, rho);
}
bool mousse::porosityModels::fixedCoeff::writeData(Ostream& os) const
{
  os  << indent << name_ << endl;
  dict_.write(os);
  return true;
}
