#ifndef CORE_PRIMITIVES_VECTOR_VECTOR_HPP_
#define CORE_PRIMITIVES_VECTOR_VECTOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   vector.cpp

#include "scalar.hpp"
#include "_vector.hpp"
#include "contiguous.hpp"


namespace mousse {

typedef Vector<scalar> vector;

//- Data associated with vector type are contiguous
template<> inline bool contiguous<vector>() {return true;}

template<class Type>
class flux
:
  public innerProduct<vector, Type>
{};


template<>
class flux<scalar>
{
public:
  typedef scalar type;
};

}  // namespace mousse
#endif
