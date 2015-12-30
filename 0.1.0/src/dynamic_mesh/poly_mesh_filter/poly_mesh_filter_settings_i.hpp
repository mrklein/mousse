// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline const mousse::dictionary&
mousse::polyMeshFilterSettings::collapseEdgesCoeffDict() const
{
  return collapseEdgesCoeffDict_;
}
inline const mousse::dictionary&
mousse::polyMeshFilterSettings::collapseFacesCoeffDict() const
{
  return collapseFacesCoeffDict_;
}
inline const mousse::dictionary&
mousse::polyMeshFilterSettings::meshQualityCoeffDict() const
{
  return meshQualityCoeffDict_;
}
inline const mousse::Switch&
mousse::polyMeshFilterSettings::controlMeshQuality() const
{
  return controlMeshQuality_;
}
inline const mousse::scalar& mousse::polyMeshFilterSettings::minLen() const
{
  return minLen_;
}
inline const mousse::scalar& mousse::polyMeshFilterSettings::maxCos() const
{
  return maxCos_;
}
inline const mousse::scalar&
mousse::polyMeshFilterSettings::edgeReductionFactor() const
{
  return edgeReductionFactor_;
}
inline const mousse::label& mousse::polyMeshFilterSettings::maxIterations() const
{
  return maxIterations_;
}
inline const mousse::label& mousse::polyMeshFilterSettings::maxSmoothIters() const
{
  return maxSmoothIters_;
}
inline const mousse::scalar&
mousse::polyMeshFilterSettings::initialFaceLengthFactor() const
{
  return initialFaceLengthFactor_;
}
inline const mousse::scalar&
mousse::polyMeshFilterSettings::faceReductionFactor() const
{
  return faceReductionFactor_;
}
inline const mousse::label&
mousse::polyMeshFilterSettings::maxPointErrorCount() const
{
  return maxPointErrorCount_;
}
