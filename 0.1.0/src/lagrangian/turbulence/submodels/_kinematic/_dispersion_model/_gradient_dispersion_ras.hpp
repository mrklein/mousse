// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GradientDispersionRAS
// Description
//   The velocity is perturbed in the direction of -grad(k), with a
//   Gaussian random number distribution with variance sigma.
//   where sigma is defined below

#ifndef _gradient_dispersion_ras_hpp_
#define _gradient_dispersion_ras_hpp_

#include "_dispersion_ras_model.hpp"

namespace mousse
{
template<class CloudType>
class GradientDispersionRAS
:
  public DispersionRASModel<CloudType>
{
protected:
  // Protected data
    // Locally cached turbulence fields
      //- Gradient of k
      const volVectorField* gradkPtr_;
      //- Take ownership of the grad(k)
      mutable bool ownGradK_;
public:
  //- Runtime type information
  TYPE_NAME("gradientDispersionRAS");
  // Constructors
    //- Construct from components
    GradientDispersionRAS(const dictionary& dict, CloudType& owner);
    //- Construct copy
    GradientDispersionRAS(const GradientDispersionRAS<CloudType>& dm);
    //- Construct and return a clone
    virtual autoPtr<DispersionModel<CloudType>> clone() const
    {
      return autoPtr<DispersionModel<CloudType>>
      {
        new GradientDispersionRAS<CloudType>{*this}
      };
    }
  //- Destructor
  virtual ~GradientDispersionRAS();
  // Member Functions
    //- Cache carrier fields
    virtual void cacheFields(const bool store);
    //- Update (disperse particles)
    virtual vector update
    (
      const scalar dt,
      const label cellI,
      const vector& U,
      const vector& Uc,
      vector& UTurb,
      scalar& tTurb
    );
};
}  // namespace mousse

#ifdef NoRepository
#include "_gradient_dispersion_ras.cpp"
#endif
#endif
