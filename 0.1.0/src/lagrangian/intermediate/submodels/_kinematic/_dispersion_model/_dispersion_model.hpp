// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DispersionModel
// Description
#ifndef _dispersion_model_hpp_
#define _dispersion_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
template<class CloudType>
class DispersionModel
:
  public CloudSubModelBase<CloudType>
{
public:
  //- Runtime type information
  TypeName("dispersionModel");
  // Declare runtime constructor selection table
  declareRunTimeSelectionTable
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
    virtual autoPtr<DispersionModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~DispersionModel();
  //- Selector
  static autoPtr<DispersionModel<CloudType> > New
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

#define makeDispersionModel(CloudType)                                        \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineTemplateTypeNameAndDebug                                              \
    (mousse::DispersionModel<kinematicCloudType>, 0);                         \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      DispersionModel<kinematicCloudType>,                                    \
      dictionary                                                              \
    );                                                                        \
  }

#define makeDispersionModelType(SS, CloudType)                                \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug(mousse::SS<kinematicCloudType>, 0);     \
                                                                              \
  mousse::DispersionModel<kinematicCloudType>::                               \
    adddictionaryConstructorToTable<mousse::SS<kinematicCloudType> >          \
      add##SS##CloudType##kinematicCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_dispersion_model.cpp"
#endif
#endif
