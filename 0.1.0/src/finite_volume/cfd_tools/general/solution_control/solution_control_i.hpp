// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline const mousse::dictionary& mousse::solutionControl::dict() const
{
  return mesh_.solutionDict().subDict(algorithmName_);
}
inline mousse::label mousse::solutionControl::corr() const
{
  return corr_;
}
inline mousse::label mousse::solutionControl::corrNonOrtho() const
{
  return corrNonOrtho_;
}
inline mousse::label mousse::solutionControl::nNonOrthCorr() const
{
  return nNonOrthCorr_;
}
inline bool mousse::solutionControl::finalNonOrthogonalIter() const
{
  return corrNonOrtho_ == nNonOrthCorr_ + 1;
}
inline bool mousse::solutionControl::momentumPredictor() const
{
  return momentumPredictor_;
}
inline bool mousse::solutionControl::transonic() const
{
  return transonic_;
}
inline bool mousse::solutionControl::consistent() const
{
  return consistent_;
}
inline bool mousse::solutionControl::correctNonOrthogonal()
{
  corrNonOrtho_++;
  if (debug)
  {
    Info<< algorithmName_ << " correctNonOrthogonal: corrNonOrtho = "
      << corrNonOrtho_ << endl;
  }
  if (corrNonOrtho_ <= nNonOrthCorr_ + 1)
  {
    return true;
  }
  else
  {
    corrNonOrtho_ = 0;
    return false;
  }
}
