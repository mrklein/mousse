// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline const mousse::dictionary& mousse::cvControls::foamyHexMeshDict() const
{
  return foamyHexMeshDict_;
}
inline mousse::scalar mousse::cvControls::pointPairDistanceCoeff() const
{
  return pointPairDistanceCoeff_;
}
inline mousse::scalar mousse::cvControls::mixedFeaturePointPPDistanceCoeff() const
{
  return mixedFeaturePointPPDistanceCoeff_;
}
inline mousse::scalar mousse::cvControls::featurePointExclusionDistanceCoeff() const
{
  return featurePointExclusionDistanceCoeff_;
}
inline mousse::scalar mousse::cvControls::featureEdgeExclusionDistanceCoeff() const
{
  return featureEdgeExclusionDistanceCoeff_;
}
inline mousse::Switch mousse::cvControls::guardFeaturePoints() const
{
  return guardFeaturePoints_;
}
inline mousse::Switch mousse::cvControls::edgeAiming() const
{
  return edgeAiming_;
}
inline mousse::Switch mousse::cvControls::snapFeaturePoints() const
{
  return snapFeaturePoints_;
}
inline mousse::Switch mousse::cvControls::circulateEdges() const
{
  return circulateEdges_;
}
inline mousse::scalar mousse::cvControls::surfacePtExclusionDistanceCoeff() const
{
  return surfacePtExclusionDistanceCoeff_;
}
inline mousse::Switch mousse::cvControls::specialiseFeaturePoints() const
{
  return specialiseFeaturePoints_;
}
inline mousse::scalar mousse::cvControls::surfaceSearchDistanceCoeff() const
{
  return surfaceSearchDistanceCoeff_;
}
inline mousse::scalar mousse::cvControls::maxSurfaceProtrusionCoeff() const
{
  return maxSurfaceProtrusionCoeff_;
}
inline mousse::scalar mousse::cvControls::maxQuadAngle() const
{
  return maxQuadAngle_;
}
inline mousse::label mousse::cvControls::surfaceConformationRebuildFrequency() const
{
  return surfaceConformationRebuildFrequency_;
}
inline mousse::scalar mousse::cvControls::edgeSearchDistCoeffSqr() const
{
  return edgeSearchDistCoeffSqr_;
}
inline mousse::scalar mousse::cvControls::surfacePtReplaceDistCoeffSqr() const
{
  return surfacePtReplaceDistCoeffSqr_;
}
inline mousse::label mousse::cvControls::maxConformationIterations() const
{
  return maxConformationIterations_;
}
inline mousse::scalar mousse::cvControls::iterationToInitialHitRatioLimit() const
{
  return iterationToInitialHitRatioLimit_;
}
inline mousse::scalar mousse::cvControls::defaultCellSize() const
{
  return defaultCellSize_;
}
inline mousse::scalar mousse::cvControls::minimumCellSize() const
{
  return minimumCellSize_;
}
inline mousse::Switch mousse::cvControls::objOutput() const
{
  return objOutput_;
}
inline mousse::Switch mousse::cvControls::timeChecks() const
{
  return timeChecks_;
}
inline mousse::Switch mousse::cvControls::printVertexInfo() const
{
  return printVertexInfo_;
}
inline mousse::scalar mousse::cvControls::maxLoadUnbalance() const
{
  return maxLoadUnbalance_;
}
inline mousse::scalar mousse::cvControls::cosAlignmentAcceptanceAngle() const
{
  return cosAlignmentAcceptanceAngle_;
}
inline mousse::scalar mousse::cvControls::insertionDistCoeff() const
{
  return insertionDistCoeff_;
}
inline mousse::scalar mousse::cvControls::faceAreaRatioCoeff() const
{
  return faceAreaRatioCoeff_;
}
inline mousse::scalar mousse::cvControls::cosInsertionAcceptanceAngle() const
{
  return cosInsertionAcceptanceAngle_;
}
inline mousse::scalar mousse::cvControls::removalDistCoeff() const
{
  return removalDistCoeff_;
}
inline mousse::Switch mousse::cvControls::filterEdges() const
{
  return filterEdges_;
}
inline mousse::Switch mousse::cvControls::filterFaces() const
{
  return filterFaces_;
}
inline mousse::Switch mousse::cvControls::writeTetDualMesh() const
{
  return writeTetDualMesh_;
}
inline mousse::Switch mousse::cvControls::writeCellShapeControlMesh() const
{
  return writeCellShapeControlMesh_;
}
inline mousse::Switch mousse::cvControls::writeBackgroundMeshDecomposition() const
{
  return writeBackgroundMeshDecomposition_;
}
