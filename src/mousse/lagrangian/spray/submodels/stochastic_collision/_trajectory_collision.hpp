#ifndef LAGRANGIAN_SPRAY_SUBMODELS_STOCHASTIC_COLLISION_TTRAJECTORY_COLLISION_HPP_
#define LAGRANGIAN_SPRAY_SUBMODELS_STOCHASTIC_COLLISION_TTRAJECTORY_COLLISION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TrajectoryCollision
// Description
//   Trajectory collision model by N. Nordin, based on O'Rourke's collision
//   model

#include "_o_rourke_collision.hpp"


namespace mousse {

template<class CloudType>
class TrajectoryCollision
:
  public ORourkeCollision<CloudType>
{
protected:
  // Protected Data
    //- Convenience typedef to the cloud's parcel type
    typedef typename CloudType::parcelType parcelType;
    //- Space coefficient
    scalar cSpace_;
    //- Time coefficient
    scalar cTime_;
  // Protected Member Functions
    //- Main collision routine
    virtual void collide(const scalar dt);
    //- Collide parcels and return true if mass has changed
    virtual bool collideParcels
    (
      const scalar dt,
      parcelType& p1,
      parcelType& p2,
      scalar& m1,
      scalar& m2
    );
public:
  //- Runtime type information
  TYPE_NAME("trajectory");
  // Constructors
    //- Construct from dictionary
    TrajectoryCollision(const dictionary& dict, CloudType& cloud);
    //- Construct copy
    TrajectoryCollision(const TrajectoryCollision<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<StochasticCollisionModel<CloudType>> clone() const
    {
      return
        autoPtr<StochasticCollisionModel<CloudType>>
        {
          new TrajectoryCollision<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~TrajectoryCollision();
};

}  // namespace mousse

#include "_trajectory_collision.ipp"

#endif
