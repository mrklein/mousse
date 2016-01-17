// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vector_2d.hpp"
namespace mousse
{
// Static Data Members
template<>
const char* const vector2D::typeName = "vector2D";
template<>
const char* vector2D::componentNames[] = {"x", "y"};
template<>
const vector2D vector2D::zero(0, 0);
template<>
const vector2D vector2D::one(1, 1);
template<>
const vector2D vector2D::max(VGREAT, VGREAT);
template<>
const vector2D vector2D::min(-VGREAT, -VGREAT);
}  // namespace mousse
