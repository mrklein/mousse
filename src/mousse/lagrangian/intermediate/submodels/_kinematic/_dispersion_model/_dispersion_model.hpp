#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TDISPERSION_MODEL_TDISPERSION_MODEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TDISPERSION_MODEL_TDISPERSION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DispersionModel

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"


namespace mousse {

template<class CloudType>
class DispersionModel
:
  public CloudSubModelBase<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("dispersionModel");
  // Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    DispersionModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    DispersionModel(CloudType& owner);
    //- Construct from components
    DispersionModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    DispersionModel(const DispersionModel<CloudType>& dm);
    //- Construct and return a clone
    virtual autoPtr<DispersionModel<CloudType>> clone() const = 0;
  //- Destructor
  virtual ~DispersionModel();
  //- Selector
  static autoPtr<DispersionModel<CloudType>> New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Member Functions
    //- Update (disperse particles)
    virtual vector update
    (
      const scalar dt,
      const label cellI,
      const vector& U,
      const vector& Uc,
      vector& UTurb,
      scalar& tTurb
    ) = 0;
};

}  // namespace mousse


#define MAKE_DISPERSION_MODEL(CloudType)                                      \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG                                         \
    (mousse::DispersionModel<kinematicCloudType>, 0);                         \
                                                                              \
  namespace mousse {                                                          \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                    \
  (                                                                           \
    DispersionModel<kinematicCloudType>,                                      \
    dictionary                                                                \
  );                                                                          \
  }


#define MAKE_DISPERSION_MODEL_TYPE(SS, CloudType)                             \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::SS<kinematicCloudType>, 0);\
                                                                              \
  mousse::DispersionModel<kinematicCloudType>::                               \
    adddictionaryConstructorToTable<mousse::SS<kinematicCloudType>>           \
      add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


#include "_dispersion_model.ipp"

#endif
