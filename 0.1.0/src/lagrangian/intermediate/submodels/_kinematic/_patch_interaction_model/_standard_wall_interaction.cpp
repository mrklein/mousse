// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_standard_wall_interaction.hpp"
// Constructors 
template<class CloudType>
mousse::StandardWallInteraction<CloudType>::StandardWallInteraction
(
  const dictionary& dict,
  CloudType& cloud
)
:
  PatchInteractionModel<CloudType>(dict, cloud, typeName),
  interactionType_
  (
    this->wordToInteractionType(this->coeffDict().lookup("type"))
  ),
  e_(0.0),
  mu_(0.0),
  nEscape_(0),
  massEscape_(0.0),
  nStick_(0),
  massStick_(0.0)
{
  switch (interactionType_)
  {
    case PatchInteractionModel<CloudType>::itOther:
    {
      const word interactionTypeName(this->coeffDict().lookup("type"));
      FatalErrorIn
      (
        "StandardWallInteraction<CloudType>::StandardWallInteraction"
        "("
          "const dictionary&, "
          "CloudType&"
        ")"
      )   << "Unknown interaction result type "
        << interactionTypeName
        << ". Valid selections are:" << this->interactionTypeNames_
        << endl << exit(FatalError);
      break;
    }
    case PatchInteractionModel<CloudType>::itRebound:
    {
      e_ = this->coeffDict().lookupOrDefault("e", 1.0);
      mu_ = this->coeffDict().lookupOrDefault("mu", 0.0);
      break;
    }
    default:
    {
      // do nothing
    }
  }
}
template<class CloudType>
mousse::StandardWallInteraction<CloudType>::StandardWallInteraction
(
  const StandardWallInteraction<CloudType>& pim
)
:
  PatchInteractionModel<CloudType>(pim),
  interactionType_(pim.interactionType_),
  e_(pim.e_),
  mu_(pim.mu_),
  nEscape_(pim.nEscape_),
  massEscape_(pim.massEscape_),
  nStick_(pim.nStick_),
  massStick_(pim.massStick_)
{}
// Destructor 
template<class CloudType>
mousse::StandardWallInteraction<CloudType>::~StandardWallInteraction()
{}
// Member Functions 
template<class CloudType>
bool mousse::StandardWallInteraction<CloudType>::correct
(
  typename CloudType::parcelType& p,
  const polyPatch& pp,
  bool& keepParticle,
  const scalar trackFraction,
  const tetIndices& tetIs
)
{
  vector& U = p.U();
  bool& active = p.active();
  if (isA<wallPolyPatch>(pp))
  {
    switch (interactionType_)
    {
      case PatchInteractionModel<CloudType>::itEscape:
      {
        keepParticle = false;
        active = false;
        U = vector::zero;
        nEscape_++;
        break;
      }
      case PatchInteractionModel<CloudType>::itStick:
      {
        keepParticle = true;
        active = false;
        U = vector::zero;
        nStick_++;
        break;
      }
      case PatchInteractionModel<CloudType>::itRebound:
      {
        keepParticle = true;
        active = true;
        vector nw;
        vector Up;
        this->owner().patchData(p, pp, trackFraction, tetIs, nw, Up);
        // Calculate motion relative to patch velocity
        U -= Up;
        scalar Un = U & nw;
        vector Ut = U - Un*nw;
        if (Un > 0)
        {
          U -= (1.0 + e_)*Un*nw;
        }
        U -= mu_*Ut;
        // Return velocity to global space
        U += Up;
        break;
      }
      default:
      {
        FatalErrorIn
        (
          "bool StandardWallInteraction<CloudType>::correct"
          "("
            "typename CloudType::parcelType&, "
            "const polyPatch&, "
            "bool& keepParticle, "
            "const scalar, "
            "const tetIndices&"
          ") const"
        )   << "Unknown interaction type "
          << this->interactionTypeToWord(interactionType_)
          << "(" << interactionType_ << ")" << endl
          << abort(FatalError);
      }
    }
    return true;
  }
  return false;
}
template<class CloudType>
void mousse::StandardWallInteraction<CloudType>::info(Ostream& os)
{
  label npe0 = this->template getBaseProperty<scalar>("nEscape");
  label npe = npe0 + returnReduce(nEscape_, sumOp<label>());
  scalar mpe0 = this->template getBaseProperty<scalar>("massEscape");
  scalar mpe = mpe0 + returnReduce(massEscape_, sumOp<scalar>());
  label nps0 = this->template getBaseProperty<scalar>("nStick");
  label nps = nps0 + returnReduce(nStick_, sumOp<label>());
  scalar mps0 = this->template getBaseProperty<scalar>("massStick");
  scalar mps = mps0 + returnReduce(massStick_, sumOp<scalar>());
  os  << "    Parcel fate (number, mass)" << nl
    << "      - escape                      = " << npe << ", " << mpe << nl
    << "      - stick                       = " << nps << ", " << mps << nl;
  if (this->outputTime())
  {
    this->setModelProperty("nEscape", npe);
    nEscape_ = 0;
    this->setModelProperty("massEscape", mpe);
    massEscape_ = 0.0;
    this->setModelProperty("nStick", nps);
    nStick_ = 0;
    this->setModelProperty("massStick", mps);
    massStick_ = 0.0;
  }
}
