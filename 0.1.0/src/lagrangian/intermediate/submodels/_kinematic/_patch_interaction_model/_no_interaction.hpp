// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoInteraction
// Description
//   Dummy class for 'none' option - will raise an error if any functions are
//   called that require return values.
#ifndef _no_interaction_hpp_
#define _no_interaction_hpp_
#include "_patch_interaction_model.hpp"
namespace mousse
{
template<class CloudType>
class NoInteraction
:
  public PatchInteractionModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoInteraction(const dictionary& dict, CloudType& cloud);
    //- Construct copy
    NoInteraction(const NoInteraction<CloudType>& pim);
    //- Construct and return a clone
    virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
    {
      return autoPtr<PatchInteractionModel<CloudType> >
      (
        new NoInteraction<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoInteraction();
  // Member Functions
    //- Flag to indicate whether model activates patch interaction model
    virtual bool active() const;
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
};
}  // namespace mousse
#ifdef NoRepository
#   include "_no_interaction.cpp"
#endif
#endif
