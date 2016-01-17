// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   label_vector.cpp
#ifndef label_vector_hpp_
#define label_vector_hpp_
#include "label.hpp"
#include "_vector.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef Vector<label> labelVector;
//- Data associated with labelVector type are contiguous
template<>
inline bool contiguous<labelVector>() {return true;}
}  // namespace mousse
#endif
