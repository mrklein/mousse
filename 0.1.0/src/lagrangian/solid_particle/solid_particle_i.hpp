// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::solidParticle::trackingData::trackingData
(
  solidParticleCloud& spc,
  const interpolationCellPoint<scalar>& rhoInterp,
  const interpolationCellPoint<vector>& UInterp,
  const interpolationCellPoint<scalar>& nuInterp,
  const vector& g
)
:
  particle::TrackingData<solidParticleCloud>(spc),
  rhoInterp_(rhoInterp),
  UInterp_(UInterp),
  nuInterp_(nuInterp),
  g_(g)
{}
inline mousse::solidParticle::solidParticle
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const scalar d,
  const vector& U
)
:
  particle(mesh, position, cellI, tetFaceI, tetPtI),
  d_(d),
  U_(U)
{}
// Member Functions 
inline const mousse::interpolationCellPoint<mousse::scalar>&
mousse::solidParticle::trackingData::rhoInterp() const
{
  return rhoInterp_;
}
inline const mousse::interpolationCellPoint<mousse::vector>&
mousse::solidParticle::trackingData::UInterp() const
{
  return UInterp_;
}
inline const mousse::interpolationCellPoint<mousse::scalar>&
mousse::solidParticle::trackingData::nuInterp() const
{
  return nuInterp_;
}
inline const mousse::vector& mousse::solidParticle::trackingData::g() const
{
  return g_;
}
inline mousse::scalar mousse::solidParticle::d() const
{
  return d_;
}
inline const mousse::vector& mousse::solidParticle::U() const
{
  return U_;
}
