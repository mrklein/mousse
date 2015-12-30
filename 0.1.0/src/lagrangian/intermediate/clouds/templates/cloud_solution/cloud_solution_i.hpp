// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::fvMesh& mousse::cloudSolution::mesh() const
{
  return mesh_;
}
inline const mousse::dictionary& mousse::cloudSolution::dict() const
{
  return dict_;
}
inline const mousse::Switch mousse::cloudSolution::active() const
{
  return active_;
}
inline const mousse::dictionary& mousse::cloudSolution::sourceTermDict() const
{
  return dict_.subDict("sourceTerms");
}
inline const mousse::dictionary& mousse::cloudSolution::interpolationSchemes() const
{
  return dict_.subDict("interpolationSchemes");
}
inline const mousse::dictionary& mousse::cloudSolution::integrationSchemes() const
{
  return dict_.subDict("integrationSchemes");
}
inline const mousse::Switch mousse::cloudSolution::transient() const
{
  return transient_;
}
inline const mousse::Switch mousse::cloudSolution::steadyState() const
{
  return !transient_;
}
inline mousse::label mousse::cloudSolution::calcFrequency() const
{
  return calcFrequency_;
}
inline mousse::scalar mousse::cloudSolution::maxCo() const
{
  return maxCo_;
}
inline mousse::label mousse::cloudSolution::iter() const
{
  return iter_;
}
inline mousse::label mousse::cloudSolution::nextIter()
{
  return ++iter_;
}
inline mousse::scalar mousse::cloudSolution::trackTime() const
{
  return trackTime_;
}
inline mousse::Switch& mousse::cloudSolution::coupled()
{
  return coupled_;
}
inline const mousse::Switch mousse::cloudSolution::coupled() const
{
  return coupled_;
}
inline const mousse::Switch mousse::cloudSolution::cellValueSourceCorrection() const
{
  return cellValueSourceCorrection_;
}
inline mousse::scalar mousse::cloudSolution::maxTrackTime() const
{
  return maxTrackTime_;
}
inline const mousse::Switch mousse::cloudSolution::resetSourcesOnStartup() const
{
  return resetSourcesOnStartup_;
}
