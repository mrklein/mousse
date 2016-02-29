#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TDEVOLATILISATION_MODEL_TDEVOLATILISATION_MODEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TDEVOLATILISATION_MODEL_TDEVOLATILISATION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DevolatilisationModel
// Description
//   Templated devolatilisation model class
// SourceFiles
//   _devolatilisation_model.cpp
//   _devolatilisation_model_new.cpp
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
template<class CloudType>
class DevolatilisationModel
:
  public CloudSubModelBase<CloudType>
{
protected:
  // Protected data
    //- Mass of lagrangian phase converted
    scalar dMass_;
public:
  //- Runtime type information
  TYPE_NAME("devolatilisationModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    DevolatilisationModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    DevolatilisationModel(CloudType& owner);
    //- Construct from dictionary
    DevolatilisationModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    DevolatilisationModel(const DevolatilisationModel<CloudType>& dm);
    //- Construct and return a clone
    virtual autoPtr<DevolatilisationModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~DevolatilisationModel();
  //- Selector
  static autoPtr<DevolatilisationModel<CloudType> > New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Member Functions
    //- Update model
    virtual void calculate
    (
      const scalar dt,
      const scalar age,
      const scalar mass0,
      const scalar mass,
      const scalar T,
      const scalarField& YGasEff,
      const scalarField& YLiquidEff,
      const scalarField& YSolidEff,
      label& canCombust,
      scalarField& dMassDV
    ) const = 0;
    //- Add to devolatilisation mass
    void addToDevolatilisationMass(const scalar dMass);
    // I-O
      //- Write injection info to stream
      virtual void info(Ostream& os);
};
}  // namespace mousse

#define MAKE_DEVOLATILISATION_MODEL(CloudType)                                \
                                                                              \
  typedef mousse::CloudType::reactingMultiphaseCloudType                      \
    reactingMultiphaseCloudType;                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::DevolatilisationModel<reactingMultiphaseCloudType>,               \
    0                                                                         \
  );                                                                          \
  namespace mousse                                                            \
  {                                                                           \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
    (                                                                         \
      DevolatilisationModel<reactingMultiphaseCloudType>,                     \
      dictionary                                                              \
    );                                                                        \
  }

#define MAKE_DEVOLATILISATION_MODEL_TYPE(SS, CloudType)                       \
                                                                              \
  typedef mousse::CloudType::reactingMultiphaseCloudType                      \
    reactingMultiphaseCloudType;                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
    (mousse::SS<reactingMultiphaseCloudType>, 0);                             \
                                                                              \
  mousse::DevolatilisationModel<reactingMultiphaseCloudType>::                \
    adddictionaryConstructorToTable                                           \
    <mousse::SS<reactingMultiphaseCloudType> >                                \
    add##SS##CloudType##reactingMultiphaseCloudType##ConstructorToTable_;

#ifdef NoRepository
#include "_devolatilisation_model.cpp"
#endif
#endif
