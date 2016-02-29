#ifndef LAGRANGIAN_SPRAY_SUBMODELS_ATOMIZATION_MODEL_TATOMIZATION_MODEL_HPP_
#define LAGRANGIAN_SPRAY_SUBMODELS_ATOMIZATION_MODEL_TATOMIZATION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AtomizationModel
// Description
//   Templated atomization model class
// SourceFiles
//   _atomization_model.cpp
//   _atomization_model_new.cpp


#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"

namespace mousse
{
template<class CloudType>
class AtomizationModel
:
  public CloudSubModelBase<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("atomizationModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    AtomizationModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    AtomizationModel(CloudType& owner);
    //- Construct from dictionary
    AtomizationModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    AtomizationModel(const AtomizationModel<CloudType>& am);
    //- Construct and return a clone
    virtual autoPtr<AtomizationModel<CloudType>> clone() const = 0;
  //- Destructor
  virtual ~AtomizationModel();
  //- Selector
  static autoPtr<AtomizationModel<CloudType>> New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Member Functions
    //- Average temperature calculation
    scalar Taverage(const scalar& Tliq, const scalar& Tc) const;
    //- Initial value of liquidCore
    virtual scalar initLiquidCore() const = 0;
    //- Flag to indicate if chi needs to be calculated
    virtual bool calcChi() const = 0;
    virtual void update
    (
      const scalar dt,
      scalar& d,
      scalar& liquidCore,
      scalar& tc,
      const scalar rho,
      const scalar mu,
      const scalar sigma,
      const scalar volFlowRate,
      const scalar rhoAv,
      const scalar Urel,
      const vector& pos,
      const vector& injectionPos,
      const scalar pAmbient,
      const scalar chi,
      cachedRandom& rndGen
    ) const = 0;
};
}  // namespace mousse

#define MAKE_ATOMIZATION_MODEL(CloudType)                                     \
                                                                              \
  typedef mousse::CloudType::sprayCloudType sprayCloudType;                   \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::AtomizationModel<sprayCloudType>,                                 \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
    (                                                                         \
      AtomizationModel<sprayCloudType>,                                       \
      dictionary                                                              \
    );                                                                        \
  }

#define MAKE_ATOMIZATION_MODEL_TYPE(SS, CloudType)                            \
                                                                              \
  typedef mousse::CloudType::sprayCloudType sprayCloudType;                   \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::SS<sprayCloudType>, 0);   \
                                                                              \
  mousse::AtomizationModel<sprayCloudType>::                                  \
    adddictionaryConstructorToTable<mousse::SS<sprayCloudType>>               \
      add##SS##CloudType##sprayCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_atomization_model.cpp"
#endif
#endif
