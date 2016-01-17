// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vector.hpp"
namespace mousse
{
// Static Data Members
template<>
const char* const vector::typeName = "vector";
template<>
const char* vector::componentNames[] = {"x", "y", "z"};
template<>
const vector vector::zero(0, 0, 0);
template<>
const vector vector::one(1, 1, 1);
template<>
const vector vector::max(VGREAT, VGREAT, VGREAT);
template<>
const vector vector::min(-VGREAT, -VGREAT, -VGREAT);
template<>
const vector vector::rootMax(ROOTVGREAT, ROOTVGREAT, ROOTVGREAT);
template<>
const vector vector::rootMin(-ROOTVGREAT, -ROOTVGREAT, -ROOTVGREAT);
}  // namespace mousse
