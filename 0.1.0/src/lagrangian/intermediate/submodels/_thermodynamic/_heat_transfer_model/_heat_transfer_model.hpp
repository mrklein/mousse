// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::HeatTransferModel
// Description
//   Templated heat transfer model class
// SourceFiles
//   _heat_transfer_model.cpp
//   _heat_transfer_model_new.cpp
#ifndef _heat_transfer_model_hpp_
#define _heat_transfer_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
template<class CloudType>
class HeatTransferModel
:
  public CloudSubModelBase<CloudType>
{
  // Private data
    //- Apply Bird's correction to the htc
    const Switch BirdCorrection_;
public:
  //- Runtime type information
  TypeName("heatTransferModel");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    HeatTransferModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    HeatTransferModel(CloudType& owner);
    //- Construct from dictionary
    HeatTransferModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    HeatTransferModel(const HeatTransferModel<CloudType>& htm);
    //- Construct and return a clone
    virtual autoPtr<HeatTransferModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~HeatTransferModel();
  //- Selector
  static autoPtr<HeatTransferModel<CloudType> > New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Member Functions
    // Access
      //- Return the Bird htc correction flag
      const Switch& BirdCorrection() const;
    // Evaluation
      //- Nusselt number
      virtual scalar Nu
      (
        const scalar Re,
        const scalar Pr
      ) const = 0;
      //- Return heat transfer coefficient
      virtual scalar htc
      (
        const scalar dp,
        const scalar Re,
        const scalar Pr,
        const scalar kappa,
        const scalar NCpW
      ) const;
};
}  // namespace mousse

#define makeHeatTransferModel(CloudType)                                      \
                                                                              \
  typedef mousse::CloudType::thermoCloudType thermoCloudType;                 \
  defineNamedTemplateTypeNameAndDebug                                         \
  (                                                                           \
    mousse::HeatTransferModel<thermoCloudType>,                               \
    0                                                                         \
  );                                                                          \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      HeatTransferModel<thermoCloudType>,                                     \
      dictionary                                                              \
    );                                                                        \
  }

#define makeHeatTransferModelType(SS, CloudType)                              \
                                                                              \
  typedef mousse::CloudType::thermoCloudType thermoCloudType;                 \
  defineNamedTemplateTypeNameAndDebug(mousse::SS<thermoCloudType>, 0);        \
                                                                              \
  mousse::HeatTransferModel<thermoCloudType>::                                \
    adddictionaryConstructorToTable<mousse::SS<thermoCloudType> >             \
      add##SS##CloudType##thermoCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_heat_transfer_model.cpp"
#endif
#endif
