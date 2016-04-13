// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sort_labelled_tri.hpp"
#include "labelled_tri.hpp"
#include "tri_surface.hpp"


namespace mousse {

// Private Classes
inline bool surfAndLabel::less::operator()
(
  const surfAndLabel& one,
  const surfAndLabel& two
) const
{
  const triSurface& surf = *one.surfPtr_;
  return surf[one.index_].region() < surf[two.index_].region();
}


// Constructors 
// Construct from components
sortLabelledTri::sortLabelledTri(const triSurface& surf)
:
  List<surfAndLabel>{surf.size(), surfAndLabel(surf, -1)}
{
  // Set the face label
  FOR_ALL(surf, faceI) {
    operator[](faceI).index_ = faceI;
  }
  // Sort according to region number.
  sort(*this, surfAndLabel::less());
}


// Member Functions 
void sortLabelledTri::indices(labelList& newIndices) const
{
  newIndices.setSize(size());
  FOR_ALL(newIndices, i) {
    newIndices[i] = operator[](i).index_;
  }
}


labelList sortLabelledTri::indices() const
{
  labelList newIndices{size()};
  indices(newIndices);
  return newIndices;
}

}  // namespace mousse
