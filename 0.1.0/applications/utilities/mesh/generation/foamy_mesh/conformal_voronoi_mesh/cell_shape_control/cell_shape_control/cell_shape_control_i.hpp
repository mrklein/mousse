// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::cellShapeControlMesh&
mousse::cellShapeControl::shapeControlMesh()
{
  return shapeControlMesh_;
}
inline const mousse::cellShapeControlMesh&
mousse::cellShapeControl::shapeControlMesh() const
{
  return shapeControlMesh_;
}
inline const mousse::scalar& mousse::cellShapeControl::defaultCellSize() const
{
  return defaultCellSize_;
}
inline const mousse::cellAspectRatioControl&
mousse::cellShapeControl::aspectRatio() const
{
  return aspectRatio_;
}
inline const mousse::cellSizeAndAlignmentControls&
mousse::cellShapeControl::sizeAndAlignment() const
{
  return sizeAndAlignment_;
}
inline const mousse::scalar& mousse::cellShapeControl::minimumCellSize() const
{
  return minimumCellSize_;
}
