// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LocalInteraction
// Description
//   Patch interaction specified on a patch-by-patch basis
#ifndef _local_interaction_hpp_
#define _local_interaction_hpp_
#include "_patch_interaction_model.hpp"
#include "patch_interaction_data_list.hpp"
#include "switch.hpp"
namespace mousse
{
template<class CloudType>
class LocalInteraction
:
  public PatchInteractionModel<CloudType>
{
  // Private data
    //- List of participating patches
    const patchInteractionDataList patchData_;
    // Counters for particle fates
      //- Number of parcels escaped
      List<label> nEscape_;
      //- Mass of parcels escaped
      List<scalar> massEscape_;
      //- Number of parcels stuck to patches
      List<label> nStick_;
      //- Mass of parcels stuck to patches
      List<scalar> massStick_;
    //- Flag to output data as fields
    Switch writeFields_;
    //- Mass escape field
    autoPtr<volScalarField> massEscapePtr_;
    //- Mass stick field
    autoPtr<volScalarField> massStickPtr_;
public:
  //- Runtime type information
  TypeName("localInteraction");
  // Constructors
    //- Construct from dictionary
    LocalInteraction(const dictionary& dict, CloudType& owner);
    //- Construct copy from owner cloud and patch interaction model
    LocalInteraction(const LocalInteraction<CloudType>& pim);
    //- Construct and return a clone using supplied owner cloud
    virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
    {
      return autoPtr<PatchInteractionModel<CloudType> >
      (
        new LocalInteraction<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~LocalInteraction();
  // Member Functions
    //- Return access to the massEscape field
    volScalarField& massEscape();
    //- Return access to the massStick field
    volScalarField& massStick();
    //- Apply velocity correction
    //  Returns true if particle remains in same cell
    virtual bool correct
    (
      typename CloudType::parcelType& p,
      const polyPatch& pp,
      bool& keepParticle,
      const scalar trackFraction,
      const tetIndices& tetIs
    );
    // I-O
      //- Write patch interaction info to stream
      virtual void info(Ostream& os);
};
}  // namespace mousse
#ifdef NoRepository
#   include "_local_interaction.cpp"
#endif
#endif
