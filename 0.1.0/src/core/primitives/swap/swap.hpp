// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef swap_hpp_
#define swap_hpp_
namespace mousse
{
template<class T>
inline void Swap(T& a, T& b)
{
  T tmp = a;
  a = b;
  b = tmp;
}
}  // namespace mousse
#endif
