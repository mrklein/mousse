// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "cell_modeller.hpp"
#include "cell_model.hpp"
// Member Functions 
inline const mousse::Map<mousse::label>& mousse::cellMatcher::localPoint() const
{
  return localPoint_;
}
inline const mousse::faceList& mousse::cellMatcher::localFaces() const
{
  return localFaces_;
}
inline const mousse::labelList& mousse::cellMatcher::faceSize() const
{
  return faceSize_;
}
inline const mousse::labelList& mousse::cellMatcher::pointMap() const
{
  return pointMap_;
}
inline const mousse::labelList& mousse::cellMatcher::faceMap() const
{
  return faceMap_;
}
inline const mousse::labelList& mousse::cellMatcher::edgeFaces() const
{
  return edgeFaces_;
}
inline const mousse::labelListList& mousse::cellMatcher::pointFaceIndex() const
{
  return pointFaceIndex_;
}
inline const mousse::labelList& mousse::cellMatcher::vertLabels() const
{
  return vertLabels_;
}
inline const mousse::labelList& mousse::cellMatcher::faceLabels() const
{
  return faceLabels_;
}
inline const mousse::cellModel& mousse::cellMatcher::model() const
{
  if (cellModelPtr_ == NULL)
  {
    cellModelPtr_ = cellModeller::lookup(cellModelName_);
  }
  return *cellModelPtr_;
}
// Key into edgeFaces_. key and key+1 are the entries for edge going from
// v0 to v1
inline mousse::label mousse::cellMatcher::edgeKey
(
  const label numVert,
  const label v0,
  const label v1
)
{
  return 2*(v0*numVert + v1);
}
// Walk along face consistent with face orientation
inline mousse::label mousse::cellMatcher::nextVert
(
  const label localVertI,
  const label size,
  const bool rightHand
)
{
  if (rightHand)
  {
    // face oriented acc. to righthand rule
    return (localVertI + 1) % size;
  }
  else
  {
    // face oriented acc. to lefthand rule
    return (size + localVertI - 1) % size;
  }
}
