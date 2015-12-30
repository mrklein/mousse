// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PairModel
// Description
//   Templated pair interaction class
// SourceFiles
//   _pair_model.cpp
//   _pair_model_new.cpp
#ifndef _pair_model_hpp_
#define _pair_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
template<class CloudType>
class PairModel
{
  // Private data
    //- The CollisionModel dictionary
    const dictionary& dict_;
    //- Reference to the owner cloud class
    CloudType& owner_;
    //- The coefficients dictionary
    const dictionary coeffDict_;
public:
  //- Runtime type information
  TypeName("pairModel");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    PairModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct from components
    PairModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
  //- Destructor
  virtual ~PairModel();
  //- Selector
  static autoPtr<PairModel<CloudType> > New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Access
    //- Return the owner cloud object
    const CloudType& owner() const;
    //- Return the dictionary
    const dictionary& dict() const;
    //- Return the coefficients dictionary
    const dictionary& coeffDict() const;
  // Member Functions
    //- Whether the PairModel has a timestep limit that will
    //  require subCycling
    virtual bool controlsTimestep() const = 0;
    //- For PairModels that control the timestep, calculate the
    //  number of subCycles needed to satisfy the minimum
    //  allowable timestep
    virtual label nSubCycles() const = 0;
    //- Calculate the pair interaction between parcels
    virtual void evaluatePair
    (
      typename CloudType::parcelType& pA,
      typename CloudType::parcelType& pB
    ) const = 0;
};
}  // namespace mousse

#define makePairModel(CloudType)                                              \
                                                                              \
  defineNamedTemplateTypeNameAndDebug                                         \
  (                                                                           \
    mousse::PairModel<mousse::CloudType>, 0                                   \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      PairModel<mousse::CloudType>,                                           \
      dictionary                                                              \
    );                                                                        \
  }

#define makePairModelType(SS, CloudType)                                      \
                                                                              \
  defineNamedTemplateTypeNameAndDebug(mousse::SS<mousse::CloudType>, 0);      \
                                                                              \
  mousse::PairModel<mousse::CloudType>::                                      \
    adddictionaryConstructorToTable<mousse::SS<mousse::CloudType> >           \
    add##SS##CloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_pair_model.cpp"
#endif
#endif
