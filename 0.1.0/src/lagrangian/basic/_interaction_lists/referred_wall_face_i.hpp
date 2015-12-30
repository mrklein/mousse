// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
const mousse::pointField& mousse::referredWallFace::points() const
{
  return pts_;
}
mousse::pointField& mousse::referredWallFace::points()
{
  return pts_;
}
mousse::label mousse::referredWallFace::patchIndex() const
{
  return patchI_;
}
mousse::label& mousse::referredWallFace::patchIndex()
{
  return patchI_;
}
