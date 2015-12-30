// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline const mousse::fvMesh& mousse::solidParticleCloud::mesh() const
{
  return mesh_;
}
inline mousse::scalar mousse::solidParticleCloud::rhop() const
{
  return rhop_;
}
inline mousse::scalar mousse::solidParticleCloud::e() const
{
  return e_;
}
inline mousse::scalar mousse::solidParticleCloud::mu() const
{
  return mu_;
}
