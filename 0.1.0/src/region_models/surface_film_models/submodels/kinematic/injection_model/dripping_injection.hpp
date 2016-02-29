#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_INJECTION_MODEL_DRIPPING_INJECTION_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_INJECTION_MODEL_DRIPPING_INJECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::drippingInjection
// Description
//   Film Dripping mass transfer model.
//   If the film mass exceeds that needed to generate a valid parcel, the
//   equivalent mass is removed from the film.
//   New parcel diameters are sampled from a PDF.
// SourceFiles
//   dripping_injection.cpp
#include "injection_model.hpp"
#include "distribution_model.hpp"
#include "cached_random.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class drippingInjection
:
  public injectionModel
{
protected:
  // Protected data
    //- Stable film thickness - drips only formed if thickness
    //  execeeds this threhold value
    scalar deltaStable_;
    //- Number of particles per parcel
    scalar particlesPerParcel_;
    //- Random number generator
    cachedRandom rndGen_;
    //- Parcel size PDF model
    const autoPtr<distributionModels::distributionModel>
      parcelDistribution_;
    //- Diameters of particles to inject into the dripping
    scalarList diameter_;
public:
  //- Runtime type information
  TYPE_NAME("drippingInjection");
  // Constructors
    //- Construct from surface film model
    drippingInjection
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    drippingInjection(const drippingInjection&) = delete;
    //- Disallow default bitwise assignment
    drippingInjection& operator=(const drippingInjection&) = delete;
  //- Destructor
  virtual ~drippingInjection();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct
      (
        scalarField& availableMass,
        scalarField& massToInject,
        scalarField& diameterToInject
      );
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
