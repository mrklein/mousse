#ifndef LAGRANGIAN_SPRAY_SUBMODELS_BREAKUP_MODEL_TBREAKUP_MODEL_HPP_
#define LAGRANGIAN_SPRAY_SUBMODELS_BREAKUP_MODEL_TBREAKUP_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::BreakupModel
// Description
//   Templated break-up model class
// SourceFiles
//   _breakup_model.cpp
//   _breakup_model_new.cpp

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"

namespace mousse
{
template<class CloudType>
class BreakupModel
:
  public CloudSubModelBase<CloudType>
{
protected:
  // Protected data
    Switch solveOscillationEq_;
    scalar y0_;
    scalar yDot0_;
    scalar TABComega_;
    scalar TABCmu_;
    scalar TABtwoWeCrit_;
public:
  //- Runtime type information
  TYPE_NAME("breakupModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    BreakupModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    BreakupModel(CloudType& owner);
    //- Construct from dictionary
    BreakupModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type,
      bool solveOscillationEq = false
    );
    //- Construct copy
    BreakupModel(const BreakupModel<CloudType>& bum);
    //- Construct and return a clone
    virtual autoPtr<BreakupModel<CloudType>> clone() const = 0;
  //- Destructor
  virtual ~BreakupModel();
  //- Selector
  static autoPtr<BreakupModel<CloudType>> New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Access
    inline const Switch& solveOscillationEq() const
    {
      return solveOscillationEq_;
    }
    inline const scalar& y0() const
    {
      return y0_;
    }
    inline const scalar& yDot0() const
    {
      return yDot0_;
    }
    inline const scalar& TABComega() const
    {
      return TABComega_;
    }
    inline const scalar& TABCmu() const
    {
      return TABCmu_;
    }
    inline const scalar& TABtwoWeCrit() const
    {
      return TABtwoWeCrit_;
    }
  // Member Functions
    //- Update the parcel properties and return true if a child parcel
    //  should be added
    virtual bool update
    (
      const scalar dt,
      const vector& g,
      scalar& d,
      scalar& tc,
      scalar& ms,
      scalar& nParticle,
      scalar& KHindex,
      scalar& y,
      scalar& yDot,
      const scalar d0,
      const scalar rho,
      const scalar mu,
      const scalar sigma,
      const vector& U,
      const scalar rhoc,
      const scalar muc,
      const vector& Urel,
      const scalar Urmag,
      const scalar tMom,
      scalar& dChild,
      scalar& massChild
    ) = 0;
};
}  // namespace mousse

#define MAKE_BREAKUP_MODEL(CloudType)                                         \
                                                                              \
  typedef mousse::CloudType::sprayCloudType sprayCloudType;                   \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::BreakupModel<sprayCloudType>,                                     \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
    (                                                                         \
      BreakupModel<sprayCloudType>,                                           \
      dictionary                                                              \
    );                                                                        \
  }

#define MAKE_BREAKUP_MODEL_TYPE(SS, CloudType)                                \
                                                                              \
  typedef mousse::CloudType::sprayCloudType sprayCloudType;                   \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::SS<sprayCloudType>, 0);   \
                                                                              \
  mousse::BreakupModel<sprayCloudType>::                                      \
    adddictionaryConstructorToTable<mousse::SS<sprayCloudType>>               \
      add##SS##CloudType##sprayCloudType##ConstructorToTable_;

#ifdef NoRepository
#include "_breakup_model.cpp"
#endif
#endif
