// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DampingModel
// Description
//   Base class for collisional damping models.
// SourceFiles
//   _damping_model.cpp
//   _damping_model_new.cpp
#ifndef _damping_model_hpp_
#define _damping_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
// Forward declaration of classes
class TimeScaleModel;
template<class CloudType>
class DampingModel
:
  public CloudSubModelBase<CloudType>
{
protected:
  // Protected data
    //- Time scale model
    autoPtr<TimeScaleModel> timeScaleModel_;
public:
  //- Runtime type information
  TYPE_NAME("dampingModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    DampingModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    DampingModel(CloudType& owner);
    //- Construct from components
    DampingModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    DampingModel(const DampingModel<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<DampingModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~DampingModel();
  //- Selector
  static autoPtr<DampingModel<CloudType> > New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Member Functions
    //- Calculate the velocity correction
    virtual vector velocityCorrection
    (
      typename CloudType::parcelType& p,
      const scalar deltaT
    ) const = 0;
};
}  // namespace mousse

#define MAKE_DAMPING_MODEL(CloudType)                                         \
                                                                              \
  typedef mousse::CloudType::MPPICCloudType MPPICCloudType;                   \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::DampingModel<MPPICCloudType>,                                     \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
    (                                                                         \
      DampingModel<MPPICCloudType>,                                           \
      dictionary                                                              \
    );                                                                        \
  }

#define MAKE_DAMPING_MODEL_TYPE(SS, CloudType)                                \
                                                                              \
  typedef mousse::CloudType::MPPICCloudType MPPICCloudType;                   \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
    (mousse::DampingModels::SS<MPPICCloudType>, 0);                           \
                                                                              \
  mousse::DampingModel<MPPICCloudType>::                                      \
    adddictionaryConstructorToTable                                           \
    <mousse::DampingModels::SS<MPPICCloudType> >                              \
      add##SS##CloudType##MPPICCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_damping_model.cpp"
#endif
#endif
