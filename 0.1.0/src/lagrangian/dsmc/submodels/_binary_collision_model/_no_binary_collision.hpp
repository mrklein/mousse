#ifndef LAGRANGIAN_DSMC_SUBMODELS_TBINARY_COLLISION_MODEL_TNO_BINARY_COLLISION_HPP_
#define LAGRANGIAN_DSMC_SUBMODELS_TBINARY_COLLISION_MODEL_TNO_BINARY_COLLISION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoBinaryCollision
// Description
//   No collison BinaryCollision Model
#include "_binary_collision_model.hpp"
namespace mousse
{
template<class CloudType>
class NoBinaryCollision
:
  public BinaryCollisionModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoBinaryCollision
    (
      const dictionary& dict,
      CloudType& cloud
    );
  //- Destructor
  virtual ~NoBinaryCollision();
  // Member Functions
    //- Flag to indicate whether model activates collision model
    virtual bool active() const;
    //- Return the collision cross section * relative velocity product
    virtual scalar sigmaTcR
    (
      const typename CloudType::parcelType& pP,
      const typename CloudType::parcelType& pQ
    ) const;
    //- Apply collision
    virtual void collide
    (
      typename CloudType::parcelType& pP,
      typename CloudType::parcelType& pQ
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "_no_binary_collision.cpp"
#endif
#endif
