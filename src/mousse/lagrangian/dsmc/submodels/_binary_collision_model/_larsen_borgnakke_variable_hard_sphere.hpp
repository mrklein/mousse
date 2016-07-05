#ifndef LAGRANGIAN_DSMC_SUBMODELS_TBINARY_COLLISION_MODEL_TLARSEN_BORGNAKKE_VARIABLE_HARD_SPHERE_HPP_
#define LAGRANGIAN_DSMC_SUBMODELS_TBINARY_COLLISION_MODEL_TLARSEN_BORGNAKKE_VARIABLE_HARD_SPHERE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LarsenBorgnakkeVariableHardSphere
// Description
//   Variable Hard Sphere BinaryCollision Model with Larsen Borgnakke internal
//   energy redistribution.  Based on the INELRS subroutine in Bird's DSMC0R.FOR

#include "_binary_collision_model.hpp"


namespace mousse {

template<class CloudType>
class LarsenBorgnakkeVariableHardSphere
:
  public BinaryCollisionModel<CloudType>
{
  // Private data
    //- Reference temperature
    const scalar Tref_;
    //- Relaxation collision number
    const scalar relaxationCollisionNumber_;
  // Private Member Functions
    //- Calculate the energy ratio for distribution to internal degrees of
    // freedom
    scalar energyRatio
    (
      scalar ChiA,
      scalar ChiB
    );
public:
  //- Runtime type information
  TYPE_NAME("LarsenBorgnakkeVariableHardSphere");
  // Constructors
    //- Construct from dictionary
    LarsenBorgnakkeVariableHardSphere
    (
      const dictionary& dict,
      CloudType& cloud
    );
  //- Destructor
  virtual ~LarsenBorgnakkeVariableHardSphere();
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

#include "_larsen_borgnakke_variable_hard_sphere.ipp"

#endif
