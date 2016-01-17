// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   vector.cpp
#ifndef vector_hpp_
#define vector_hpp_
#include "scalar.hpp"
#include "_vector.hpp"
#include "contiguous.hpp"
namespace mousse
{
typedef Vector<scalar> vector;
//- Data associated with vector type are contiguous
template<>
inline bool contiguous<vector>() {return true;}
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
