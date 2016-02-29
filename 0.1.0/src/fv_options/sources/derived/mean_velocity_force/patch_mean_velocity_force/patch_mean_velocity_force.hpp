#ifndef FV_OPTIONS_SOURCES_DERIVED_MEAN_VELOCITY_FORCE_PATCH_MEAN_VELOCITY_FORCE_PATCH_MEAN_VELOCITY_FORCE_HPP_
#define FV_OPTIONS_SOURCES_DERIVED_MEAN_VELOCITY_FORCE_PATCH_MEAN_VELOCITY_FORCE_PATCH_MEAN_VELOCITY_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::patchMeanVelocityForce
// Description
//   Calculates and applies the force necessary to maintain the specified mean
//   velocity averaged over the specified patch.
//   Note: Currently only handles kinematic pressure (incompressible solvers).
//   \heading Source usage
//   Example usage:
//   \verbatim
//   patchMeanVelocityForceCoeffs
//   {
//     selectionMode   all;                    // Apply force to all cells
//     fieldNames      (U);                    // Name of velocity field
//     patch           inlet;                  // Name of the patch
//     Ubar            (10.0 0 0);             // Desired mean velocity
//     relaxation      0.2;                    // Optional relaxation factor
//   }
//   \endverbatim
// SourceFiles
//   patch_mean_velocity_force.cpp
#include "mean_velocity_force.hpp"
namespace mousse
{
namespace fv
{
class patchMeanVelocityForce
:
  public meanVelocityForce
{
protected:
  // Protected data
    //- Patch name
    word patch_;
    //- Patch index
    label patchi_;
  // Protected Member Functions
    //- Calculate and return the magnitude of the mean velocity
    //  averaged over the specified patch
    virtual scalar magUbarAve(const volVectorField& U) const;
public:
  //- Runtime type information
  TYPE_NAME("patchMeanVelocityForce");
  // Constructors
    //- Construct from explicit source name and mesh
    patchMeanVelocityForce
    (
      const word& sourceName,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
    //- Disallow default bitwise copy construct
    patchMeanVelocityForce(const patchMeanVelocityForce&) = delete;
    //- Disallow default bitwise assignment
    patchMeanVelocityForce& operator=(const patchMeanVelocityForce&) = delete;
};
}  // namespace fv
}  // namespace mousse
#endif
