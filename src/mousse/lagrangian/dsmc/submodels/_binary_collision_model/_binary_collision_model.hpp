#ifndef LAGRANGIAN_DSMC_SUBMODELS_TBINARY_COLLISION_MODEL_TBINARY_COLLISION_MODEL_HPP_
#define LAGRANGIAN_DSMC_SUBMODELS_TBINARY_COLLISION_MODEL_TBINARY_COLLISION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::BinaryCollisionModel
// Description
//   Templated DSMC particle collision class

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

template<class CloudType>
class BinaryCollisionModel
{
  // Private data
    //- The cloud dictionary
    const dictionary& dict_;
    // reference to the owner cloud class
    CloudType& owner_;
    //- The coefficients dictionary
    const dictionary coeffDict_;
public:
  //- Runtime type information
  TYPE_NAME("BinaryCollisionModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    BinaryCollisionModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    BinaryCollisionModel(CloudType& owner);
    //- Construct from components
    BinaryCollisionModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
  //- Destructor
  virtual ~BinaryCollisionModel();
  //- Selector
  static autoPtr<BinaryCollisionModel<CloudType>> New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Access
    //- Return the owner cloud object
    const CloudType& owner() const;
    //- Return non-const access to the owner cloud object
    CloudType& owner();
    //- Return the dictionary
    const dictionary& dict() const;
    //- Return the coefficients dictionary
    const dictionary& coeffDict() const;
  // Member Functions
    //- Flag to indicate whether model activates collision model
    virtual bool active() const = 0;
    //- Return the collision cross section * relative velocity product
    virtual scalar sigmaTcR
    (
      const typename CloudType::parcelType& pP,
      const typename CloudType::parcelType& pQ
    ) const = 0;
    //- Apply collision
    virtual void collide
    (
      typename CloudType::parcelType& pP,
      typename CloudType::parcelType& pQ
    ) = 0;
};

}  // namespace mousse


#define MAKE_BINARY_COLLISION_MODEL(CloudType)                                \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    BinaryCollisionModel<CloudType>,                                          \
    0                                                                         \
  );                                                                          \
                                                                              \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                    \
  (                                                                           \
    BinaryCollisionModel<CloudType>,                                          \
    dictionary                                                                \
  );


#define MAKE_BINARY_COLLISION_MODEL_TYPE(SS, CloudType)                       \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(SS<CloudType>, 0);                \
                                                                              \
  BinaryCollisionModel<CloudType>::                                           \
    adddictionaryConstructorToTable<SS<CloudType>>                            \
      add##SS##CloudType##ConstructorToTable_;


#include "_binary_collision_model.ipp"

#endif
