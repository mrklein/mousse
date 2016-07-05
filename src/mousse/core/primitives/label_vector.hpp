#ifndef CORE_PRIMITIVES_VECTOR_LABEL_VECTOR_HPP_
#define CORE_PRIMITIVES_VECTOR_LABEL_VECTOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   label_vector.cpp

#include "label.hpp"
#include "_vector.hpp"
#include "contiguous.hpp"


namespace mousse {

typedef Vector<label> labelVector;

//- Data associated with labelVector type are contiguous
template<> inline bool contiguous<labelVector>() {return true;}

}  // namespace mousse

#endif
