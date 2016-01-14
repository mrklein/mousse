// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoCollision
// Description
//   Place holder for 'none' option
// SourceFiles
//   _no_collision.cpp
#ifndef _no_collision_hpp_
#define _no_collision_hpp_
#include "_collision_model.hpp"
namespace mousse
{
template<class CloudType>
class NoCollision
:
  public CollisionModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    NoCollision(const dictionary& dict, CloudType& owner);
    //- Construct copy
    NoCollision(const NoCollision<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<CollisionModel<CloudType> > clone() const
    {
      return autoPtr<CollisionModel<CloudType> >
      (
        new NoCollision<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoCollision();
  // Member Functions
    //- Return the number of times to subcycle the current
    //  timestep to meet the criteria of the collision model.  For
    //  this model this will always be 1.
    virtual label nSubCycles() const;
    //- Flag to indicate whether model activates collision model
    virtual bool active() const;
    //- Indicates whether model determines wall collisions or not,
    //  used to determine what value to use for wallImpactDistance
    virtual bool controlsWallInteraction() const;
    // Collision function
    virtual void collide();
};
}  // namespace mousse
#ifdef NoRepository
#   include "_no_collision.cpp"
#endif
#endif
