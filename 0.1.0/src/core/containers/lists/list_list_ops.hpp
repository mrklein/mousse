// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   list_list_ops.cpp
#ifndef list_list_ops_hpp_
#define list_list_ops_hpp_
#include "label_list.hpp"
namespace mousse
{
// Dummy access operator for ListListOps::combine()
template<class T>
class accessOp
{
public:
  const T& operator()(const T& x) const
  {
    return x;
  }
};
// Offset operator for ListListOps::combineOffset()
template<class T>
class offsetOp
{
public:
  T operator()(const T& x, const label offset) const
  {
    return x + offset;
  }
};
namespace ListListOps
{
  //- Combines sublists into one big list
  template<class AccessType, class T, class AccessOp>
  AccessType combine(const List<T>&, AccessOp aop = accessOp<T>());
  //- Gets sizes of sublists
  template<class T, class AccessOp>
  labelList subSizes(const List<T>&, AccessOp aop = accessOp<T>());
  //- Like combine but also offsets sublists based on passed sizes
  template<class AccessType, class T, class AccessOp, class OffsetOp>
  AccessType combineOffset
  (
    const List<T>&,
    const labelList& sizes,
    AccessOp aop,
    OffsetOp oop = offsetOp<T>()
  );
};
}  // namespace mousse
#ifdef NoRepository
#   include "list_list_ops.cpp"
#endif
#endif
