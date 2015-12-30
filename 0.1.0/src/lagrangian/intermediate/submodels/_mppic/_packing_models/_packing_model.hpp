// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PackingModel
// Description
//   Base class for packing models.
// SourceFiles
//   _packing_model.cpp
//   _packing_model_new.cpp
#ifndef _packing_model_hpp_
#define _packing_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
// Forward declaration of classes
class ParticleStressModel;
class CorrectionLimitingMethod;
template <class Type>
class AveragingMethod;
template<class CloudType>
class PackingModel
:
  public CloudSubModelBase<CloudType>
{
protected:
  //- Protected data
    //- Particle stress model
    autoPtr<ParticleStressModel> particleStressModel_;
public:
  //- Runtime type information
  TypeName("packingModel");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    PackingModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    PackingModel(CloudType& owner);
    //- Construct from components
    PackingModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    PackingModel(const PackingModel<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<PackingModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~PackingModel();
  //- Selector
  static autoPtr<PackingModel<CloudType> > New
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

#define makePackingModel(CloudType)                                           \
                                                                              \
  typedef mousse::CloudType::MPPICCloudType MPPICCloudType;                   \
  defineNamedTemplateTypeNameAndDebug                                         \
  (                                                                           \
    mousse::PackingModel<MPPICCloudType>,                                     \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      PackingModel<MPPICCloudType>,                                           \
      dictionary                                                              \
    );                                                                        \
  }

#define makePackingModelType(SS, CloudType)                                   \
                                                                              \
  typedef mousse::CloudType::MPPICCloudType MPPICCloudType;                   \
  defineNamedTemplateTypeNameAndDebug                                         \
    (mousse::PackingModels::SS<MPPICCloudType>, 0);                           \
                                                                              \
  mousse::PackingModel<MPPICCloudType>::                                      \
    adddictionaryConstructorToTable                                           \
    <mousse::PackingModels::SS<MPPICCloudType> >                              \
      add##SS##CloudType##MPPICCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_packing_model.cpp"
#endif
#endif
