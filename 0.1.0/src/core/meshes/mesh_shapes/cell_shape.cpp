// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_shape.hpp"
#include "degenerate_matcher.hpp"


// Member Functions 

// Remove duplicate vertices and reset correct cell model
void mousse::cellShape::collapse()
{
  operator=(degenerateMatcher::match(*this));
}
