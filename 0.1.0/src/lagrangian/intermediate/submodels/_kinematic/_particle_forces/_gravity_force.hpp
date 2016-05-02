#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TGRAVITY_FORCE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TGRAVITY_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GravityForce
// Description
//   Calculates particle gravity force

#include "_particle_force.hpp"


namespace mousse {

class fvMesh;


template<class CloudType>
class GravityForce
:
  public ParticleForce<CloudType>
{
  // Private data
    //- Reference to the acceleration due to gravity
    const vector& g_;
public:
  //- Runtime type information
  TYPE_NAME("gravity");
  // Constructors
    //- Construct from mesh
    GravityForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    GravityForce(const GravityForce& gf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType>> clone() const
    {
      return
        autoPtr<ParticleForce<CloudType>>
        {
          new GravityForce<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~GravityForce();
  // Member Functions
    // Access
      //- Return the acceleration due to gravity
      inline const vector& g() const;
    // Evaluation
      //- Calculate the non-coupled force
      virtual forceSuSp calcNonCoupled
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
inline const mousse::vector& mousse::GravityForce<CloudType>::g() const
{
  return g_;
}

#include "_gravity_force.ipp"

#endif

