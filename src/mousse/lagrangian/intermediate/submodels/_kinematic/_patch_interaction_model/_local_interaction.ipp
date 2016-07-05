// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_local_interaction.hpp"


// Constructors 
template<class CloudType>
mousse::LocalInteraction<CloudType>::LocalInteraction
(
  const dictionary& dict,
  CloudType& cloud
)
:
  PatchInteractionModel<CloudType>{dict, cloud, typeName},
  patchData_{cloud.mesh(), this->coeffDict()},
  nEscape_{patchData_.size(), 0},
  massEscape_{patchData_.size(), 0.0},
  nStick_{patchData_.size(), 0},
  massStick_{patchData_.size(), 0.0},
  writeFields_{this->coeffDict().lookupOrDefault("writeFields", false)},
  massEscapePtr_{nullptr},
  massStickPtr_{nullptr}
{
  if (writeFields_) {
    word massEscapeName{this->owner().name() + ":massEscape"};
    word massStickName{this->owner().name() + ":massStick"};
    Info << "    Interaction fields will be written to " << massEscapeName
      << " and " << massStickName << endl;
    (void)massEscape();
    (void)massStick();
  } else {
    Info<< "    Interaction fields will not be written" << endl;
  }
  // check that interactions are valid/specified
  FOR_ALL(patchData_, patchI) {
    const word& interactionTypeName =
      patchData_[patchI].interactionTypeName();
    const typename PatchInteractionModel<CloudType>::interactionType& it =
      this->wordToInteractionType(interactionTypeName);
    if (it == PatchInteractionModel<CloudType>::itOther) {
      const word& patchName = patchData_[patchI].patchName();
      FATAL_ERROR_IN("LocalInteraction(const dictionary&, CloudType&)")
        << "Unknown patch interaction type "
        << interactionTypeName << " for patch " << patchName
        << ". Valid selections are:"
        << this->PatchInteractionModel<CloudType>::interactionTypeNames_
        << nl << exit(FatalError);
    }
  }
}


template<class CloudType>
mousse::LocalInteraction<CloudType>::LocalInteraction
(
  const LocalInteraction<CloudType>& pim
)
:
  PatchInteractionModel<CloudType>{pim},
  patchData_{pim.patchData_},
  nEscape_{pim.nEscape_},
  massEscape_{pim.massEscape_},
  nStick_{pim.nStick_},
  massStick_{pim.massStick_},
  writeFields_{pim.writeFields_},
  massEscapePtr_{nullptr},
  massStickPtr_{nullptr}
{}


// Destructor 
template<class CloudType>
mousse::LocalInteraction<CloudType>::~LocalInteraction()
{}


// Member Functions 
template<class CloudType>
mousse::volScalarField& mousse::LocalInteraction<CloudType>::massEscape()
{
  if (!massEscapePtr_.valid()) {
    const fvMesh& mesh = this->owner().mesh();
    massEscapePtr_.reset
    (
      new volScalarField
      {
        {
          this->owner().name() + ":massEscape",
          mesh.time().timeName(),
          mesh,
          IOobject::READ_IF_PRESENT,
          IOobject::AUTO_WRITE
        },
        mesh,
        {"zero", dimMass, 0.0}
      }
    );
  }
  return massEscapePtr_();
}


template<class CloudType>
mousse::volScalarField& mousse::LocalInteraction<CloudType>::massStick()
{
  if (!massStickPtr_.valid()) {
    const fvMesh& mesh = this->owner().mesh();
    massStickPtr_.reset
    (
      new volScalarField
      {
        {
          this->owner().name() + ":massStick",
          mesh.time().timeName(),
          mesh,
          IOobject::READ_IF_PRESENT,
          IOobject::AUTO_WRITE
        },
        mesh,
        {"zero", dimMass, 0.0}
      }
    );
  }
  return massStickPtr_();
}


template<class CloudType>
bool mousse::LocalInteraction<CloudType>::correct
(
  typename CloudType::parcelType& p,
  const polyPatch& pp,
  bool& keepParticle,
  const scalar trackFraction,
  const tetIndices& tetIs
)
{
  label patchI = patchData_.applyToPatch(pp.index());
  if (patchI >= 0) {
    vector& U = p.U();
    bool& active = p.active();
    typename PatchInteractionModel<CloudType>::interactionType it =
      this->wordToInteractionType
      (
        patchData_[patchI].interactionTypeName()
      );
    switch (it) {
      case PatchInteractionModel<CloudType>::itEscape:
        {
          scalar dm = p.mass()*p.nParticle();
          keepParticle = false;
          active = false;
          U = vector::zero;
          nEscape_[patchI]++;
          massEscape_[patchI] += dm;
          if (writeFields_) {
            label pI = pp.index();
            label fI = pp.whichFace(p.face());
            massEscape().boundaryField()[pI][fI] += dm;
          }
          break;
        }
      case PatchInteractionModel<CloudType>::itStick:
        {
          scalar dm = p.mass()*p.nParticle();
          keepParticle = true;
          active = false;
          U = vector::zero;
          nStick_[patchI]++;
          massStick_[patchI] += dm;
          if (writeFields_) {
            label pI = pp.index();
            label fI = pp.whichFace(p.face());
            massStick().boundaryField()[pI][fI] += dm;
          }
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
          if (Un > 0) {
            U -= (1.0 + patchData_[patchI].e())*Un*nw;
          }
          U -= patchData_[patchI].mu()*Ut;
          // Return velocity to global space
          U += Up;
          break;
        }
      default:
        {
          FATAL_ERROR_IN
          (
            "bool LocalInteraction<CloudType>::correct"
            "("
            "typename CloudType::parcelType&, "
            "const polyPatch&, "
            "bool&, "
            "const scalar, "
            "const tetIndices&"
            ") const"
          )
          << "Unknown interaction type "
          << patchData_[patchI].interactionTypeName()
          << "(" << it << ") for patch "
          << patchData_[patchI].patchName()
          << ". Valid selections are:" << this->interactionTypeNames_
          << endl << abort(FatalError);
        }
    }
    return true;
  }
  return false;
}


template<class CloudType>
void mousse::LocalInteraction<CloudType>::info(Ostream& os)
{
  // retrieve any stored data
  labelList npe0{patchData_.size(), 0};
  this->getModelProperty("nEscape", npe0);
  scalarList mpe0{patchData_.size(), 0.0};
  this->getModelProperty("massEscape", mpe0);
  labelList nps0{patchData_.size(), 0};
  this->getModelProperty("nStick", nps0);
  scalarList mps0{patchData_.size(), 0.0};
  this->getModelProperty("massStick", mps0);
  // accumulate current data
  labelList npe{nEscape_};
  Pstream::listCombineGather(npe, plusEqOp<label>());
  npe = npe + npe0;
  scalarList mpe{massEscape_};
  Pstream::listCombineGather(mpe, plusEqOp<scalar>());
  mpe = mpe + mpe0;
  labelList nps(nStick_);
  Pstream::listCombineGather(nps, plusEqOp<label>());
  nps = nps + nps0;
  scalarList mps{massStick_};
  Pstream::listCombineGather(mps, plusEqOp<scalar>());
  mps = mps + mps0;
  FOR_ALL(patchData_, i) {
    os << "    Parcel fate (number, mass)      : patch "
      <<  patchData_[i].patchName() << nl
      << "      - escape                      = " << npe[i]
      << ", " << mpe[i] << nl
      << "      - stick                       = " << nps[i]
      << ", " << mps[i] << nl;
  }
  if (this->outputTime()) {
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
