// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::BrownianMotionForce
// Description
//   Calculates particle Brownian motion force
// SourceFiles
//   _brownian_motion_force.cpp

#ifndef _brownian_motion_force_hpp_
#define _brownian_motion_force_hpp_

#include "_particle_force.hpp"
#include "cached_random.hpp"

namespace mousse
{
template<class CloudType>
class BrownianMotionForce
:
  public ParticleForce<CloudType>
{
  // Private data
    //- Reference to the cloud random number generator
    cachedRandom& rndGen_;
    //- Molecular free path length [m]
    const scalar lambda_;
    //- Turbulence flag
    bool turbulence_;
    //- Pointer to the turbulence kinetic energy field
    const volScalarField* kPtr_;
    //- Flag that indicates ownership of turbulence k field
    bool ownK_;
  // Private Member Functions
    //- Inverse erf for Gaussian distribution
    scalar erfInv(const scalar y) const;
    //- Return the k field from the turbulence model
    tmp<volScalarField> kModel() const;
public:
  //- Runtime type information
  TYPE_NAME("BrownianMotion");
  // Constructors
    //- Construct from mesh
    BrownianMotionForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    BrownianMotionForce(const BrownianMotionForce& bmf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType>> clone() const
    {
      return autoPtr<ParticleForce<CloudType>>
      {
        new BrownianMotionForce<CloudType>{*this}
      };
    }
  //- Destructor
  virtual ~BrownianMotionForce();
  // Member Functions
    // Access
      //- Return const access to the molecular free path length [m]
      inline scalar lambda() const;
      //- Return const access to the turbulence flag
      inline bool turbulence() const;
    // Evaluation
      //- Cache fields
      virtual void cacheFields(const bool store);
      //- Calculate the non-coupled force
      virtual forceSuSp calcCoupled
      (
        const typename CloudType::parcelType& p,
        const scalar dt,
        const scalar mass,
        const scalar Re,
        const scalar muc
      ) const;
};
}  // namespace mousse

template<class CloudType>
inline mousse::scalar mousse::BrownianMotionForce<CloudType>::lambda() const
{
  return lambda_;
}
template<class CloudType>
inline bool mousse::BrownianMotionForce<CloudType>::turbulence() const
{
  return turbulence_;
}

#ifdef NoRepository
  #include "_brownian_motion_force.cpp"
#endif
#endif
