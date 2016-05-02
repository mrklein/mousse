#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TLIFT_TSAFFMAN_MEI_LIFT_FORCE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TLIFT_TSAFFMAN_MEI_LIFT_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SaffmanMeiLiftForce
// Description
//   Saffman-Mei particle lift force model applicable to spherical particles.

#include "_lift_force.hpp"


namespace mousse {

template<class CloudType>
class SaffmanMeiLiftForce
:
  public LiftForce<CloudType>
{
protected:
  // Protected Member Functions
    //- Calculate the lift coefficient
    virtual scalar Cl
    (
      const typename CloudType::parcelType& p,
      const vector& curlUc,
      const scalar Re,
      const scalar muc
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("SaffmanMeiLiftForce");
  // Constructors
    //- Construct from mesh
    SaffmanMeiLiftForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& forceType = typeName
    );
    //- Construct copy
    SaffmanMeiLiftForce(const SaffmanMeiLiftForce& lf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType>> clone() const
    {
      return
        autoPtr<ParticleForce<CloudType>>
        {
          new SaffmanMeiLiftForce<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~SaffmanMeiLiftForce();
};

}  // namespace mousse

#include "_saffman_mei_lift_force.ipp"

#endif

