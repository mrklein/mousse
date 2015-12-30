// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermocapillary_force.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fvc_grad.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
defineTypeNameAndDebug(thermocapillaryForce, 0);
addToRunTimeSelectionTable(force, thermocapillaryForce, dictionary);
// Constructors 
thermocapillaryForce::thermocapillaryForce
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  force(owner)
{}
// Destructor 
thermocapillaryForce::~thermocapillaryForce()
{}
// Member Functions 
tmp<fvVectorMatrix> thermocapillaryForce::correct(volVectorField& U)
{
  const volScalarField& sigma = owner_.sigma();
  tmp<fvVectorMatrix>
    tfvm(new fvVectorMatrix(U, dimForce/dimArea*dimVolume));
  tfvm() += fvc::grad(sigma);
  return tfvm;
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
