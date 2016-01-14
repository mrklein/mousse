// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SurfaceReactionModel
// Description
//   Templated surface reaction model class
// SourceFiles
//   _surface_reaction_model.cpp
//   _surface_reaction_model_new.cpp
#ifndef _surface_reaction_model_hpp_
#define _surface_reaction_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
#include "scalar_field.hpp"
namespace mousse
{
template<class CloudType>
class SurfaceReactionModel
:
  public CloudSubModelBase<CloudType>
{
protected:
  // Protected data
    //- Mass of lagrangian phase converted
    scalar dMass_;
public:
  //-Runtime type information
  TYPE_NAME("surfaceReactionModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    SurfaceReactionModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& cloud
    ),
    (dict, cloud)
  );
  // Constructors
    //- Construct null from owner
    SurfaceReactionModel(CloudType& owner);
    //- Construct from dictionary
    SurfaceReactionModel
    (
      const dictionary& dict,
      CloudType& cloud,
      const word& type
    );
    //- Construct copy
    SurfaceReactionModel(const SurfaceReactionModel<CloudType>& srm);
    //- Construct and return a clone
    virtual autoPtr<SurfaceReactionModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~SurfaceReactionModel();
  //- Selector
  static autoPtr<SurfaceReactionModel<CloudType> > New
  (
    const dictionary& dict,
    CloudType& cloud
  );
  // Member Functions
    //- Update surface reactions
    //  Returns the heat of reaction
    virtual scalar calculate
    (
      const scalar dt,
      const label cellI,
      const scalar d,
      const scalar T,
      const scalar Tc,
      const scalar pc,
      const scalar rhoc,
      const scalar mass,
      const scalarField& YGas,
      const scalarField& YLiquid,
      const scalarField& YSolid,
      const scalarField& YMixture,
      const scalar N,
      scalarField& dMassGas,
      scalarField& dMassLiquid,
      scalarField& dMassSolid,
      scalarField& dMassSRCarrier
    ) const = 0;
    //- Add to devolatilisation mass
    void addToSurfaceReactionMass(const scalar dMass);
    // I-O
      //- Write injection info to stream
      virtual void info(Ostream& os);
};
}  // namespace mousse

#define MAKE_SURFACE_REACTION_MODEL(CloudType)                                \
                                                                              \
  typedef mousse::CloudType::reactingMultiphaseCloudType                      \
    reactingMultiphaseCloudType;                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::SurfaceReactionModel<reactingMultiphaseCloudType>,                \
    0                                                                         \
  );                                                                          \
  namespace mousse                                                            \
  {                                                                           \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
    (                                                                         \
      SurfaceReactionModel<reactingMultiphaseCloudType>,                      \
      dictionary                                                              \
    );                                                                        \
  }

#define MAKE_SURFACE_REACTION_MODEL_TYPE(SS, CloudType)                       \
                                                                              \
  typedef mousse::CloudType::reactingMultiphaseCloudType                      \
    reactingMultiphaseCloudType;                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
    (mousse::SS<reactingMultiphaseCloudType>, 0);                             \
                                                                              \
  mousse::SurfaceReactionModel<reactingMultiphaseCloudType>::                 \
    adddictionaryConstructorToTable                                           \
    <mousse::SS<reactingMultiphaseCloudType> >                                \
    add##SS##CloudType##reactingMultiphaseCloudType##ConstructorToTable_;

#ifdef NoRepository
#include "_surface_reaction_model.cpp"
#endif
#endif
