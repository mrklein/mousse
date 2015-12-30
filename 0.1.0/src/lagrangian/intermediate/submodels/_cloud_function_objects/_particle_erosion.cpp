// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_particle_erosion.hpp"
// Protectd Member Functions
template<class CloudType>
mousse::label mousse::ParticleErosion<CloudType>::applyToPatch
(
  const label globalPatchI
) const
{
  forAll(patchIDs_, i)
  {
    if (patchIDs_[i] == globalPatchI)
    {
      return i;
    }
  }
  return -1;
}
template<class CloudType>
void mousse::ParticleErosion<CloudType>::write()
{
  if (QPtr_.valid())
  {
    QPtr_->write();
  }
  else
  {
    FatalErrorIn("void mousse::ParticleErosion<CloudType>::write()")
      << "QPtr not valid" << abort(FatalError);
  }
}
// Constructors 
template<class CloudType>
mousse::ParticleErosion<CloudType>::ParticleErosion
(
  const dictionary& dict,
  CloudType& owner,
  const word& modelName
)
:
  CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
  QPtr_(NULL),
  patchIDs_(),
  p_(readScalar(this->coeffDict().lookup("p"))),
  psi_(this->coeffDict().template lookupOrDefault<scalar>("psi", 2.0)),
  K_(this->coeffDict().template lookupOrDefault<scalar>("K", 2.0))
{
  const wordList allPatchNames = owner.mesh().boundaryMesh().names();
  wordList patchName(this->coeffDict().lookup("patches"));
  labelHashSet uniquePatchIDs;
  forAllReverse(patchName, i)
  {
    labelList patchIDs = findStrings(patchName[i], allPatchNames);
    if (patchIDs.empty())
    {
      WarningIn
      (
        "mousse::ParticleErosion<CloudType>::ParticleErosion"
        "("
          "const dictionary&, "
          "CloudType& "
        ")"
      )   << "Cannot find any patch names matching " << patchName[i]
        << endl;
    }
    uniquePatchIDs.insert(patchIDs);
  }
  patchIDs_ = uniquePatchIDs.toc();
  // trigger ther creation of the Q field
  preEvolve();
}
template<class CloudType>
mousse::ParticleErosion<CloudType>::ParticleErosion
(
  const ParticleErosion<CloudType>& pe
)
:
  CloudFunctionObject<CloudType>(pe),
  QPtr_(NULL),
  patchIDs_(pe.patchIDs_),
  p_(pe.p_),
  psi_(pe.psi_),
  K_(pe.K_)
{}
// Destructor 
template<class CloudType>
mousse::ParticleErosion<CloudType>::~ParticleErosion()
{}
// Member Functions 
template<class CloudType>
void mousse::ParticleErosion<CloudType>::preEvolve()
{
  if (QPtr_.valid())
  {
    QPtr_->internalField() = 0.0;
  }
  else
  {
    const fvMesh& mesh = this->owner().mesh();
    QPtr_.reset
    (
      new volScalarField
      (
        IOobject
        (
          this->owner().name() + "Q",
          mesh.time().timeName(),
          mesh,
          IOobject::READ_IF_PRESENT,
          IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dimVolume, 0.0)
      )
    );
  }
}
template<class CloudType>
void mousse::ParticleErosion<CloudType>::postPatch
(
  const parcelType& p,
  const polyPatch& pp,
  const scalar trackFraction,
  const tetIndices& tetIs,
  bool&
)
{
  const label patchI = pp.index();
  const label localPatchI = applyToPatch(patchI);
  if (localPatchI != -1)
  {
    vector nw;
    vector Up;
    // patch-normal direction
    this->owner().patchData(p, pp, trackFraction, tetIs, nw, Up);
    // particle velocity reletive to patch
    const vector& U = p.U() - Up;
    // quick reject if particle travelling away from the patch
    if ((nw & U) < 0)
    {
      return;
    }
    const scalar magU = mag(U);
    const vector Udir = U/magU;
    // determine impact angle, alpha
    const scalar alpha = mathematical::pi/2.0 - acos(nw & Udir);
    const scalar coeff = p.nParticle()*p.mass()*sqr(magU)/(p_*psi_*K_);
    const label patchFaceI = pp.whichFace(p.face());
    scalar& Q = QPtr_->boundaryField()[patchI][patchFaceI];
    if (tan(alpha) < K_/6.0)
    {
      Q += coeff*(sin(2.0*alpha) - 6.0/K_*sqr(sin(alpha)));
    }
    else
    {
      Q += coeff*(K_*sqr(cos(alpha))/6.0);
    }
  }
}
