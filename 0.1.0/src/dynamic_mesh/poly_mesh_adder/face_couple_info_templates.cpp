// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_couple_info.hpp"
// Private Member Functions 
// Add a T entry
template<template<class> class FaceList>
mousse::pointField mousse::faceCoupleInfo::calcFaceCentres
(
  const FaceList<face>& faces,
  const pointField& points,
  const label start,
  const label size
)
{
  pointField fc(size);
  label faceI = start;
  forAll(fc, i)
  {
    fc[i] = faces[faceI++].centre(points);
  }
  return fc;
}
