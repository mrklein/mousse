// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IsotropyModels::Stochastic
// Description
//   Stochastic return-to-isotropy model.
//   Particle velocities are modified by sampling a gaussian-plus-delta
//   distribution, which depends on a time-scale. This randomises some particle
//   velocities whilst leaving others unchanged. The lower the value of the
//   time-scale, the greater the proportion of particle velocities affected.
//   A correction step is performed at the end to ensure that the model
//   conserves both momentum and granular temperature.
//   Reference:
//   \verbatim
//     "Inclusion of collisional return-to-isotropy in the MP-PIC method"
//     P O'Rourke and D Snider
//     Chemical Engineering Science
//     Volume 80, Issue 0, Pages 39-54, December 2012
//   \endverbatim
// SourceFiles
//   stochastic.cpp
#ifndef _stochastic_hpp_
#define _stochastic_hpp_
#include "_isotropy_model.hpp"
namespace mousse
{
namespace IsotropyModels
{
template<class CloudType>
class Stochastic
:
  public IsotropyModel<CloudType>
{
private:
  // Private Member Functions
    //- Sample a gaussian distribution using the Box-Muller method
    scalar sampleGauss();
public:
  //- Runtime type information
  TYPE_NAME("stochastic");
  // Constructors
    //- Construct from components
    Stochastic(const dictionary& dict, CloudType& owner);
    //- Construct copy
    Stochastic(const Stochastic<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<IsotropyModel<CloudType> > clone() const
    {
      return autoPtr<IsotropyModel<CloudType> >
      (
        new Stochastic<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~Stochastic();
  //- Member Functions
    //- Calculate velocities
    virtual void calculate();
};
}  // namespace IsotropyModels
}  // namespace mousse
#ifdef NoRepository
#   include "_stochastic.cpp"
#endif
#endif
