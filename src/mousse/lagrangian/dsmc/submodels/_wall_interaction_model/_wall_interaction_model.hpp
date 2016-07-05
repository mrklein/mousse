#ifndef LAGRANGIAN_DSMC_SUBMODELS_TWALL_INTERACTION_MODEL_TWALL_INTERACTION_MODEL_HPP_
#define LAGRANGIAN_DSMC_SUBMODELS_TWALL_INTERACTION_MODEL_TWALL_INTERACTION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::WallInteractionModel
// Description
//   Templated wall interaction model class

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

template<class CloudType>
class WallInteractionModel
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
  TYPE_NAME("WallInteractionModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    WallInteractionModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    WallInteractionModel(CloudType& owner);
    //- Construct from components
    WallInteractionModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
  //- Destructor
  virtual ~WallInteractionModel();
  //- Selector
  static autoPtr<WallInteractionModel<CloudType> > New
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
    //- Apply wall correction
    virtual void correct
    (
      typename CloudType::parcelType& p,
      const wallPolyPatch& wpp
    ) = 0;
};

}  // namespace mousse


#define MAKE_WALL_INTERACTION_MODEL(CloudType)                                \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(WallInteractionModel<CloudType>, 0);\
                                                                              \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                    \
  (                                                                           \
    WallInteractionModel<CloudType>,                                          \
    dictionary                                                                \
  );

#define MAKE_WALL_INTERACTION_MODEL_TYPE(SS, CloudType)                       \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(SS<CloudType>, 0);                \
                                                                              \
  WallInteractionModel<CloudType>::                                           \
    adddictionaryConstructorToTable<SS<CloudType>>                            \
      add##SS##CloudType##ConstructorToTable_;


#include "_wall_interaction_model.ipp"

#endif
