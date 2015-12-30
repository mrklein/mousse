// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IsotropyModel
// Description
//   Base class for collisional return-to-isotropy models.
// SourceFiles
//   _isotropy_model.cpp
//   _isotropy_model_new.cpp
#ifndef _isotropy_model_hpp_
#define _isotropy_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
// Forward declaration of classes
class TimeScaleModel;
template<class CloudType>
class IsotropyModel
:
  public CloudSubModelBase<CloudType>
{
protected:
    //- Time scale model
    autoPtr<TimeScaleModel> timeScaleModel_;
public:
  //- Runtime type information
  TypeName("isotropyModel");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    IsotropyModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    IsotropyModel(CloudType& owner);
    //- Construct from components
    IsotropyModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct a copy
    IsotropyModel(const IsotropyModel<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<IsotropyModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~IsotropyModel();
  //- Selector
  static autoPtr<IsotropyModel<CloudType> > New
  (
    const dictionary& dict,
    CloudType& owner
  );
  //- Member Functions
    //- Calculate velocities
    virtual void calculate() = 0;
};
}  // namespace mousse

#define makeIsotropyModel(CloudType)                                          \
                                                                              \
  typedef mousse::CloudType::MPPICCloudType MPPICCloudType;                   \
  defineNamedTemplateTypeNameAndDebug                                         \
  (                                                                           \
    mousse::IsotropyModel<MPPICCloudType>,                                    \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      IsotropyModel<MPPICCloudType>,                                          \
      dictionary                                                              \
    );                                                                        \
  }

#define makeIsotropyModelType(SS, CloudType)                                  \
                                                                              \
  typedef mousse::CloudType::MPPICCloudType MPPICCloudType;                   \
  defineNamedTemplateTypeNameAndDebug                                         \
    (mousse::IsotropyModels::SS<MPPICCloudType>, 0);                          \
                                                                              \
  mousse::IsotropyModel<MPPICCloudType>::                                     \
    adddictionaryConstructorToTable                                           \
    <mousse::IsotropyModels::SS<MPPICCloudType> >                             \
      add##SS##CloudType##MPPICCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_isotropy_model.cpp"
#endif
#endif
