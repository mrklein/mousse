// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PhaseChangeModel
// Description
//   Templated phase change model class
// SourceFiles
//   _phase_change_model.cpp
//   _phase_change_model_new.cpp
#ifndef _phase_change_model_hpp_
#define _phase_change_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
template<class CloudType>
class PhaseChangeModel
:
  public CloudSubModelBase<CloudType>
{
public:
  // Public enumerations
    //- Enthalpy transfer type
    enum enthalpyTransferType
    {
      etLatentHeat,
      etEnthalpyDifference
    };
    //- Name representations of enthalpy transfer types
    static const mousse::wordList enthalpyTransferTypeNames;
protected:
  // Protected data
    //- Enthalpy transfer type enumeration
    enthalpyTransferType enthalpyTransfer_;
    // Counters
      //- Mass of lagrangian phase converted
      scalar dMass_;
  // Protected Member Functions
    //- Convert word to enthalpy transfer type
    enthalpyTransferType wordToEnthalpyTransfer(const word& etName) const;
    //- Sherwood number
    scalar Sh() const;
public:
  //- Runtime type information
  TYPE_NAME("phaseChangeModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    PhaseChangeModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    PhaseChangeModel(CloudType& owner);
    //- Construct from dictionary
    PhaseChangeModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    PhaseChangeModel(const PhaseChangeModel<CloudType>& pcm);
    //- Construct and return a clone
    virtual autoPtr<PhaseChangeModel<CloudType>> clone() const = 0;
  //- Destructor
  virtual ~PhaseChangeModel();
  //- Selector
  static autoPtr<PhaseChangeModel<CloudType>> New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Access
    //- Return the enthalpy transfer type enumeration
    const enthalpyTransferType& enthalpyTransfer() const;
  // Member Functions
    //- Update model
    virtual void calculate
    (
      const scalar dt,
      const label cellI,
      const scalar Re,
      const scalar Pr,
      const scalar d,
      const scalar nu,
      const scalar T,
      const scalar Ts,
      const scalar pc,
      const scalar Tc,
      const scalarField& X,
      scalarField& dMassPC
    ) const = 0;
    //- Return the enthalpy per unit mass
    virtual scalar dh
    (
      const label idc,
      const label idl,
      const scalar p,
      const scalar T
    ) const;
    //- Return vapourisation temperature
    virtual scalar Tvap(const scalarField& X) const;
    //- Return maximum/limiting temperature
    virtual scalar TMax(const scalar p, const scalarField& X) const;
    //- Add to phase change mass
    void addToPhaseChangeMass(const scalar dMass);
    // I-O
      //- Write injection info to stream
      virtual void info(Ostream& os);
};
}  // namespace mousse

#define MAKE_PHASE_CHANGE_MODEL(CloudType)                                    \
                                                                              \
  typedef mousse::CloudType::reactingCloudType reactingCloudType;             \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::PhaseChangeModel<reactingCloudType>,                              \
    0                                                                         \
  );                                                                          \
  namespace mousse                                                            \
  {                                                                           \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
    (                                                                         \
      PhaseChangeModel<reactingCloudType>,                                    \
      dictionary                                                              \
    );                                                                        \
  }

#define MAKE_PHASE_CHANGE_MODEL_TYPE(SS, CloudType)                           \
                                                                              \
  typedef mousse::CloudType::reactingCloudType reactingCloudType;             \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::SS<reactingCloudType>, 0);\
                                                                              \
  mousse::PhaseChangeModel<reactingCloudType>::                               \
    adddictionaryConstructorToTable<mousse::SS<reactingCloudType>>            \
      add##SS##CloudType##reactingCloudType##ConstructorToTable_;

#ifdef NoRepository
#include "_phase_change_model.cpp"
#endif
#endif
