// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::VariableHardSphere
// Description
//   Variable Hard Sphere BinaryCollision Model
#ifndef _variable_hard_sphere_hpp_
#define _variable_hard_sphere_hpp_
#include "_binary_collision_model.hpp"
namespace mousse
{
template<class CloudType>
class VariableHardSphere
:
  public BinaryCollisionModel<CloudType>
{
  // Private data
    //- Reference temperature
    const scalar Tref_;
public:
  //- Runtime type information
  TypeName("VariableHardSphere");
  // Constructors
    //- Construct from dictionary
    VariableHardSphere
    (
      const dictionary& dict,
      CloudType& cloud
    );
  //- Destructor
  virtual ~VariableHardSphere();
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
#   include "_variable_hard_sphere.cpp"
#endif
#endif
