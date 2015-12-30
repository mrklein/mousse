// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::scalar mousse::pairPotential::rMin() const
{
  return rMin_;
}
inline mousse::scalar mousse::pairPotential::dr() const
{
  return dr_;
}
inline mousse::scalar mousse::pairPotential::rCut() const
{
  return rCut_;
}
inline mousse::scalar mousse::pairPotential::rCutSqr() const
{
  return rCutSqr_;
}
inline bool mousse::pairPotential::writeTables() const
{
  return writeTables_;
}
