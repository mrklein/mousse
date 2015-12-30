// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_mean_velocity_force.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Member Functions
namespace mousse
{
namespace fv
{
  defineTypeNameAndDebug(patchMeanVelocityForce, 0);
  addToRunTimeSelectionTable
  (
    option,
    patchMeanVelocityForce,
    dictionary
  );
}
}
// Constructors 
mousse::fv::patchMeanVelocityForce::patchMeanVelocityForce
(
  const word& sourceName,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  meanVelocityForce(sourceName, modelType, dict, mesh),
  patch_(coeffs_.lookup("patch")),
  patchi_(mesh.boundaryMesh().findPatchID(patch_))
{
  if (patchi_ < 0)
  {
    FatalErrorIn("fv::patchMeanVelocityForce::patchMeanVelocityForce")
      << "Cannot find patch " << patch_
      << exit(FatalError);
  }
}
// Member Functions 
mousse::scalar mousse::fv::patchMeanVelocityForce::magUbarAve
(
  const volVectorField& U
) const
{
  return
    gSum
    (
      (flowDir_ & U.boundaryField()[patchi_])
     *mesh_.boundary()[patchi_].magSf()
    )/gSum(mesh_.boundary()[patchi_].magSf());
}
