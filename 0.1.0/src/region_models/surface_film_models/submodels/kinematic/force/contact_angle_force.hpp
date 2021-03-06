#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FORCE_CONTACT_ANGLE_FORCE_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FORCE_CONTACT_ANGLE_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::contactAngleForce
// Description
//   Film contact angle force
//   The effect of the contact angle force can be ignored over a specified
//   distance from patches.

#include "force.hpp"
#include "distribution_model.hpp"
#include "cached_random.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

class contactAngleForce
:
  public force
{
private:
  // Private Data
    //- Coefficient applied to the contact angle force
    scalar Ccf_;
    //- Random number generator
    cachedRandom rndGen_;
    //- Parcel size PDF model
    const autoPtr<distributionModels::distributionModel> distribution_;
    //- Mask over which force is applied
    volScalarField mask_;
  // Private member functions
    //- Initialise
    void initialise();
public:
  //- Runtime type information
  TYPE_NAME("contactAngle");
  // Constructors
    //- Construct from surface film model
    contactAngleForce
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    contactAngleForce(const contactAngleForce&) = delete;
    //- Disallow default bitwise assignment
    contactAngleForce& operator=(const contactAngleForce&) = delete;
  //- Destructor
  virtual ~contactAngleForce();
  // Member Functions
    // Evolution
      //- Correct
      virtual tmp<fvVectorMatrix> correct(volVectorField& U);
};

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

#endif

