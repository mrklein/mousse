// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face.hpp"
// Private Member Functions 
inline bool mousse::polyTopoChange::pointRemoved(const label pointI) const
{
  const point& pt = points_[pointI];
  return
    pt.x() > 0.5*vector::max.x()
  && pt.y() > 0.5*vector::max.y()
  && pt.z() > 0.5*vector::max.z();
}
inline bool mousse::polyTopoChange::faceRemoved(const label faceI) const
{
  return faces_[faceI].empty();
}
inline bool mousse::polyTopoChange::cellRemoved(const label cellI) const
{
  return cellMap_[cellI] == -2;
}
inline void mousse::polyTopoChange::setNumPatches(const label nPatches)
{
  nPatches_ = nPatches;
}
