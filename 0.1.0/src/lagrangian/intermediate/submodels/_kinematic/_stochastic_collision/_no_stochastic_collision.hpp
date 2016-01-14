// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoStochasticCollision
// Description
//   Dummy collision model for 'none'
#ifndef _no_stochastic_collision_hpp_
#define _no_stochastic_collision_hpp_
#include "_stochastic_collision_model.hpp"
namespace mousse
{
template<class CloudType>
class NoStochasticCollision
:
  public StochasticCollisionModel<CloudType>
{
protected:
  // Protected Member Functions
    //- Update the model
    virtual void collide(const scalar dt);
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoStochasticCollision(const dictionary& dict, CloudType& owner);
    //- Construct copy
    NoStochasticCollision(const NoStochasticCollision<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<StochasticCollisionModel<CloudType>> clone() const
    {
      return autoPtr<StochasticCollisionModel<CloudType>>
      {
        new NoStochasticCollision<CloudType>{*this}
      };
    }
  //- Destructor
  virtual ~NoStochasticCollision();
  // Member Functions
    //- Flag to indicate whether model activates collision model
    virtual bool active() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_no_stochastic_collision.cpp"
#endif
#endif
