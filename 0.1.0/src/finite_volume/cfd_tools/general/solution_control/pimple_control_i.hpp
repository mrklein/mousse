// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::label mousse::pimpleControl::nCorrPIMPLE() const
{
  return nCorrPIMPLE_;
}
inline mousse::label mousse::pimpleControl::nCorrPISO() const
{
  return nCorrPISO_;
}
inline mousse::label mousse::pimpleControl::corrPISO() const
{
  return corrPISO_;
}
inline bool mousse::pimpleControl::correct()
{
  corrPISO_++;
  if (debug)
  {
    Info<< algorithmName_ << " correct: corrPISO = " << corrPISO_ << endl;
  }
  if (corrPISO_ <= nCorrPISO_)
  {
    return true;
  }
  else
  {
    corrPISO_ = 0;
    return false;
  }
}
inline bool mousse::pimpleControl::storeInitialResiduals() const
{
  // Start from second PIMPLE iteration
  return (corr_ == 2) && (corrPISO_ == 0) && (corrNonOrtho_ == 0);
}
inline bool mousse::pimpleControl::firstIter() const
{
  return corr_ == 1;
}
inline bool mousse::pimpleControl::finalIter() const
{
  return converged_ || (corr_ == nCorrPIMPLE_);
}
inline bool mousse::pimpleControl::finalInnerIter() const
{
  return
   finalIter()
  && corrPISO_ == nCorrPISO_
  && corrNonOrtho_ == nNonOrthCorr_ + 1;
}
inline bool mousse::pimpleControl::turbCorr() const
{
  return !turbOnFinalIterOnly_ || finalIter();
}
