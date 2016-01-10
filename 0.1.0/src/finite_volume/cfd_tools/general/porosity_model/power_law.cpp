// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "power_law.hpp"
#include "geometric_one_field.hpp"
#include "fv_matrices.hpp"
// Static Data Members
namespace mousse
{
  namespace porosityModels
  {
    DEFINE_TYPE_NAME_AND_DEBUG(powerLaw, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE(porosityModel, powerLaw, mesh);
  }
}
// Constructors 
mousse::porosityModels::powerLaw::powerLaw
(
  const word& name,
  const word& modelType,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& cellZoneName
)
:
  porosityModel(name, modelType, mesh, dict, cellZoneName),
  C0_(readScalar(coeffs_.lookup("C0"))),
  C1_(readScalar(coeffs_.lookup("C1"))),
  rhoName_(coeffs_.lookupOrDefault<word>("rho", "rho"))
{}
// Destructor 
mousse::porosityModels::powerLaw::~powerLaw()
{}
// Member Functions 
void mousse::porosityModels::powerLaw::calcTranformModelData()
{
  // nothing to be transformed
}
void mousse::porosityModels::powerLaw::calcForce
(
  const volVectorField& U,
  const volScalarField& rho,
  const volScalarField& /*mu*/,
  vectorField& force
) const
{
  scalarField Udiag(U.size(), 0.0);
  const scalarField& V = mesh_.V();
  apply(Udiag, V, rho, U);
  force = Udiag*U;
}
void mousse::porosityModels::powerLaw::correct
(
  fvVectorMatrix& UEqn
) const
{
  const vectorField& U = UEqn.psi();
  const scalarField& V = mesh_.V();
  scalarField& Udiag = UEqn.diag();

  if (UEqn.dimensions() == dimForce)
  {
    const volScalarField& rho =
      mesh_.lookupObject<volScalarField>(rhoName_);
    apply(Udiag, V, rho, U);
  }
  else
  {
    apply(Udiag, V, geometricOneField(), U);
  }
}
void mousse::porosityModels::powerLaw::correct
(
  fvVectorMatrix& UEqn,
  const volScalarField& rho,
  const volScalarField& /*mu*/
) const
{
  const vectorField& U = UEqn.psi();
  const scalarField& V = mesh_.V();
  scalarField& Udiag = UEqn.diag();

  apply(Udiag, V, rho, U);
}
void mousse::porosityModels::powerLaw::correct
(
  const fvVectorMatrix& UEqn,
  volTensorField& AU
) const
{
  const vectorField& U = UEqn.psi();
  if (UEqn.dimensions() == dimForce)
  {
    const volScalarField& rho =
      mesh_.lookupObject<volScalarField>(rhoName_);
    apply(AU, rho, U);
  }
  else
  {
    apply(AU, geometricOneField(), U);
  }
}
bool mousse::porosityModels::powerLaw::writeData(Ostream& os) const
{
  os  << indent << name_ << endl;
  dict_.write(os);
  return true;
}
