// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PatchInteractionModel
// Description
//   Templated patch interaction model class
// SourceFiles
//   _patch_interaction_model.cpp
//   _patch_interaction_model_new.cpp
#ifndef _patch_interaction_model_hpp_
#define _patch_interaction_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "poly_patch.hpp"
#include "wall_poly_patch.hpp"
#include "tet_indices.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
template<class CloudType>
class PatchInteractionModel
:
  public CloudSubModelBase<CloudType>
{
public:
  // Public enumerations
    // Interaction types
    enum interactionType
    {
      itRebound,
      itStick,
      itEscape,
      itOther
    };
    static wordList interactionTypeNames_;
private:
  // Private data
    //- Name of velocity field - default = "U"
    const word UName_;
public:
  //- Runtime type information
  TypeName("patchInteractionModel");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    PatchInteractionModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    PatchInteractionModel(CloudType& owner);
    //- Construct from components
    PatchInteractionModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    PatchInteractionModel(const PatchInteractionModel<CloudType>& pim);
    //- Construct and return a clone
    virtual autoPtr<PatchInteractionModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~PatchInteractionModel();
  //- Selector
  static autoPtr<PatchInteractionModel<CloudType> > New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Access
    //- Return name of velocity field
    const word& UName() const;
  // Member Functions
    //- Convert interaction result to word
    static word interactionTypeToWord(const interactionType& itEnum);
    //- Convert word to interaction result
    static interactionType wordToInteractionType(const word& itWord);
    //- Apply velocity correction
    //  Returns true if particle remains in same cell
    virtual bool correct
    (
      typename CloudType::parcelType& p,
      const polyPatch& pp,
      bool& keepParticle,
      const scalar trackFraction,
      const tetIndices& tetIs
    ) = 0;
    // I-O
      //- Write patch interaction info to stream
      virtual void info(Ostream& os);
};
}  // namespace mousse

#define makePatchInteractionModel(CloudType)                                  \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug                                         \
  (                                                                           \
    mousse::PatchInteractionModel<kinematicCloudType>,                        \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      PatchInteractionModel<kinematicCloudType>,                              \
      dictionary                                                              \
    );                                                                        \
  }

#define makePatchInteractionModelType(SS, CloudType)                          \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug(mousse::SS<kinematicCloudType>, 0);     \
                                                                              \
  mousse::PatchInteractionModel<kinematicCloudType>::                         \
    adddictionaryConstructorToTable<mousse::SS<kinematicCloudType> >          \
      add##SS##CloudType##kinematicCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_patch_interaction_model.cpp"
#endif
#endif
