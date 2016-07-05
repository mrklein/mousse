#ifndef LAGRANGIAN_TURBULENCE_SUBMODELS_TKINEMATIC_TDISPERSION_MODEL_TSTOCHASTIC_DISPERSION_RAS_HPP_
#define LAGRANGIAN_TURBULENCE_SUBMODELS_TKINEMATIC_TDISPERSION_MODEL_TSTOCHASTIC_DISPERSION_RAS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::StochasticDispersionRAS
// Description
//   The velocity is perturbed in random direction, with a
//   Gaussian random number distribution with variance sigma.
//   where sigma is defined below

#include "_dispersion_ras_model.hpp"


namespace mousse {

template<class CloudType>
class StochasticDispersionRAS
:
  public DispersionRASModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("stochasticDispersionRAS");
  // Constructors
    //- Construct from components
    StochasticDispersionRAS(const dictionary& dict, CloudType& owner);
    //- Construct copy
    StochasticDispersionRAS(const StochasticDispersionRAS<CloudType>& dm);
    //- Construct and return a clone
    virtual autoPtr<DispersionModel<CloudType>> clone() const
    {
      return
        autoPtr<DispersionModel<CloudType>>
        {
          new StochasticDispersionRAS<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~StochasticDispersionRAS();
  // Member Functions
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

#include "_stochastic_dispersion_ras.ipp"

#endif
