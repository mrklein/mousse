// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ORourkeCollision
// Description
//   Collision model by P.J. O'Rourke.
#ifndef _o_rourke_collision_hpp_
#define _o_rourke_collision_hpp_

#include "_stochastic_collision_model.hpp"
#include "liquid_mixture_properties.hpp"

namespace mousse
{
template<class CloudType>
class ORourkeCollision
:
  public StochasticCollisionModel<CloudType>
{
protected:
  // Protected Data
    //- Convenience typedef to the cloud's parcel type
    typedef typename CloudType::parcelType parcelType;
    const liquidMixtureProperties& liquids_;
    //- Coalescence activation switch
    Switch coalescence_;
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
    // 1 is the larger drop and 2 is the smaller
    virtual bool collideSorted
    (
      const scalar dt,
      parcelType& p1,
      parcelType& p2,
      scalar& m1,
      scalar& m2
    );
public:
  //- Runtime type information
  TYPE_NAME("ORourke");
  // Constructors
    //- Construct from dictionary
    ORourkeCollision
    (
      const dictionary& dict,
      CloudType& cloud,
      const word& modelName = typeName
    );
    //- Construct copy
    ORourkeCollision(const ORourkeCollision<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<StochasticCollisionModel<CloudType> > clone() const
    {
      return autoPtr<StochasticCollisionModel<CloudType> >
      (
        new ORourkeCollision<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ORourkeCollision();
};
}  // namespace mousse
#ifdef NoRepository
#   include "_o_rourke_collision.cpp"
#endif
#endif
