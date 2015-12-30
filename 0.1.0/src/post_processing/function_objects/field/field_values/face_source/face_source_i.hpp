// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_source.hpp"
// Member Functions 
inline const mousse::fieldValues::faceSource::sourceType&
mousse::fieldValues::faceSource::source() const
{
  return source_;
}
inline const mousse::labelList&
mousse::fieldValues::faceSource::faceId() const
{
  return faceId_;
}
inline const mousse::labelList&
mousse::fieldValues::faceSource::facePatch() const
{
  return facePatchId_;
}
inline const mousse::labelList&
mousse::fieldValues::faceSource::faceSign() const
{
  return faceSign_;
}
