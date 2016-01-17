// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
template<class T>
inline const T mousse::SVD::sign(const T& a, const T& b)
{
  return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}
// Member Functions 
inline const mousse::scalarRectangularMatrix& mousse::SVD::U() const
{
  return U_;
}
inline const mousse::scalarRectangularMatrix& mousse::SVD::V() const
{
  return V_;
}
inline const mousse::scalarDiagonalMatrix& mousse::SVD::S() const
{
  return S_;
}
inline const mousse::scalarRectangularMatrix& mousse::SVD::VSinvUt() const
{
  return VSinvUt_;
}
inline mousse::label mousse::SVD::nZeros() const
{
  return nZeros_;
}
inline mousse::scalar mousse::SVD::minNonZeroS() const
{
  scalar minS = S_[0];
  for (label i = 1; i < S_.size(); i++)
  {
    scalar s = S_[i];
    if (s > VSMALL && s < minS) minS = s;
  }
  return minS;
}
