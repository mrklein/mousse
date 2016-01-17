// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline mousse::scalar mousse::cv2DControls::minCellSize() const
{
  return minCellSize_;
}
inline mousse::scalar mousse::cv2DControls::minCellSize2() const
{
  return minCellSize2_;
}
inline mousse::scalar mousse::cv2DControls::maxQuadAngle() const
{
  return maxQuadAngle_;
}
inline mousse::scalar mousse::cv2DControls::nearWallAlignedDist() const
{
  return nearWallAlignedDist_;
}
inline mousse::scalar mousse::cv2DControls::nearWallAlignedDist2() const
{
  return nearWallAlignedDist2_;
}
inline mousse::Switch mousse::cv2DControls::insertSurfaceNearestPointPairs() const
{
  return insertSurfaceNearestPointPairs_;
}
inline mousse::Switch mousse::cv2DControls::mirrorPoints() const
{
  return mirrorPoints_;
}
inline mousse::Switch mousse::cv2DControls::insertSurfaceNearPointPairs() const
{
  return insertSurfaceNearPointPairs_;
}
inline mousse::Switch mousse::cv2DControls::objOutput() const
{
  return objOutput_;
}
inline mousse::Switch mousse::cv2DControls::meshedSurfaceOutput() const
{
  return meshedSurfaceOutput_;
}
inline mousse::Switch mousse::cv2DControls::randomiseInitialGrid() const
{
  return randomiseInitialGrid_;
}
inline mousse::scalar mousse::cv2DControls::randomPerturbation() const
{
  return randomPerturbation_;
}
inline mousse::label mousse::cv2DControls::maxBoundaryConformingIter() const
{
  return maxBoundaryConformingIter_;
}
inline mousse::scalar mousse::cv2DControls::span() const
{
  return span_;
}
inline mousse::scalar mousse::cv2DControls::span2() const
{
  return span2_;
}
inline mousse::scalar mousse::cv2DControls::minEdgeLen() const
{
  return minEdgeLen_;
}
inline mousse::scalar mousse::cv2DControls::minEdgeLen2() const
{
  return minEdgeLen2_;
}
inline mousse::scalar mousse::cv2DControls::maxNotchLen() const
{
  return maxNotchLen_;
}
inline mousse::scalar mousse::cv2DControls::maxNotchLen2() const
{
  return maxNotchLen2_;
}
inline mousse::scalar mousse::cv2DControls::minNearPointDist() const
{
  return minNearPointDist_;
}
inline mousse::scalar mousse::cv2DControls::minNearPointDist2() const
{
  return minNearPointDist2_;
}
inline mousse::scalar mousse::cv2DControls::ppDist() const
{
  return ppDist_;
}
