#ifndef CORE_PRIMITIVES_SWAP_SWAP_HPP_
#define CORE_PRIMITIVES_SWAP_SWAP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


namespace mousse {

template<class T>
inline void Swap(T& a, T& b)
{
  T tmp = a;
  a = b;
  b = tmp;
}

}  // namespace mousse

#endif
