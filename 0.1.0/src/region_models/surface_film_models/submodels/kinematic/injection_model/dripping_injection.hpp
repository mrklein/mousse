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
#ifndef dripping_injection_hpp_
#define dripping_injection_hpp_
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
private:
  // Private member functions
    //- Disallow default bitwise copy construct
    drippingInjection(const drippingInjection&);
    //- Disallow default bitwise assignment
    void operator=(const drippingInjection&);
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
  TypeName("drippingInjection");
  // Constructors
    //- Construct from surface film model
    drippingInjection
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
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
