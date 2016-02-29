#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FORCE_THERMOCAPILLARY_FORCE_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FORCE_THERMOCAPILLARY_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermocapillaryForce
// Description
//   Thermocapillary force
// SourceFiles
//   thermocapillary_force.cpp
#include "force.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class thermocapillaryForce
:
  public force
{
public:
  //- Runtime type information
  TYPE_NAME("thermocapillary");
  // Constructors
    //- Construct from surface film model
    thermocapillaryForce
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    thermocapillaryForce(const thermocapillaryForce&) = delete;
    //- Disallow default bitwise assignment
    thermocapillaryForce& operator=(const thermocapillaryForce&) = delete;
  //- Destructor
  virtual ~thermocapillaryForce();
  // Member Functions
    // Evolution
      //- Correct
      virtual tmp<fvVectorMatrix> correct(volVectorField& U);
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
