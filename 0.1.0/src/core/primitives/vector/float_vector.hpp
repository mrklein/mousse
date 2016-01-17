// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   float_vector.cpp
#ifndef float_vector_hpp_
#define float_vector_hpp_
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
