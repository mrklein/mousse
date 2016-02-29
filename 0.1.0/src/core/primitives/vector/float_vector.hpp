#ifndef CORE_PRIMITIVES_VECTOR_FLOAT_VECTOR_HPP_
#define CORE_PRIMITIVES_VECTOR_FLOAT_VECTOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   float_vector.cpp
#include "_vector.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef Vector<float> floatVector;
//- Data associated with floatVector type are contiguous
#if !defined(WM_SP)
template<>
inline bool contiguous<floatVector>() {return true;}
#endif
}  // namespace mousse
#endif
