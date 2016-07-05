// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "list_ops.hpp"


// Global Functions 
template<class ListType>
ListType mousse::renumber
(
  const labelUList& oldToNew,
  const ListType& lst
)
{
  // Create copy
  ListType newLst{lst.size()};
  // ensure consistent addressable size {eg, DynamicList}
  newLst.setSize(lst.size());
  FOR_ALL(lst, elemI) {
    if (lst[elemI] >= 0) {
      newLst[elemI] = oldToNew[lst[elemI]];
    }
  }
  return newLst;
}


template<class ListType>
void mousse::inplaceRenumber
(
  const labelUList& oldToNew,
  ListType& lst
)
{
  FOR_ALL(lst, elemI) {
    if (lst[elemI] >= 0) {
      lst[elemI] = oldToNew[lst[elemI]];
    }
  }
}


template<class ListType>
ListType mousse::reorder
(
  const labelUList& oldToNew,
  const ListType& lst
)
{
  // Create copy
  ListType newLst{lst.size()};
  // ensure consistent addressable size {eg, DynamicList}
  newLst.setSize(lst.size());
  FOR_ALL(lst, elemI) {
    if (oldToNew[elemI] >= 0) {
      newLst[oldToNew[elemI]] = lst[elemI];
    } else {
      newLst[elemI] = lst[elemI];
    }
  }
  return newLst;
}


template<class ListType>
void mousse::inplaceReorder
(
  const labelUList& oldToNew,
  ListType& lst
)
{
  // Create copy
  ListType newLst{lst.size()};
  // ensure consistent addressable size {eg, DynamicList}
  newLst.setSize(lst.size());
  FOR_ALL(lst, elemI) {
    if (oldToNew[elemI] >= 0) {
      newLst[oldToNew[elemI]] = lst[elemI];
    } else {
      newLst[elemI] = lst[elemI];
    }
  }
  lst.transfer(newLst);
}


template<class Container>
void mousse::inplaceMapValue
(
  const labelUList& oldToNew,
  Container& lst
)
{
  for
  (
    typename Container::iterator iter = lst.begin();
    iter != lst.end();
    ++iter
  ) {
    if (iter() >= 0) {
      iter() = oldToNew[iter()];
    }
  }
}


template<class Container>
void mousse::inplaceMapKey
(
  const labelUList& oldToNew,
  Container& lst
)
{
  Container newLst{lst.size()};
  for
  (
    typename Container::iterator iter = lst.begin();
    iter != lst.end();
    ++iter
  ) {
    if (iter.key() >= 0) {
      newLst.insert(oldToNew[iter.key()], iter());
    }
  }
  lst.transfer(newLst);
}


template<class T>
void mousse::sortedOrder
(
  const UList<T>& lst,
  labelList& order
)
{
  sortedOrder(lst, order, typename UList<T>::less(lst));
}


template<class T, class Cmp>
void mousse::sortedOrder
(
  const UList<T>& lst,
  labelList& order,
  const Cmp& cmp
)
{
  // list lengths must be identical
  if (order.size() != lst.size()) {
    // avoid copying any elements, they are overwritten anyhow
    order.clear();
    order.setSize(lst.size());
  }
  FOR_ALL(order, elemI) {
    order[elemI] = elemI;
  }
  mousse::stableSort(order, cmp);
}


template<class T>
void mousse::duplicateOrder
(
  const UList<T>& lst,
  labelList& order
)
{
  duplicateOrder(lst, order, typename UList<T>::less(lst));
}


template<class T, class Cmp>
void mousse::duplicateOrder
(
  const UList<T>& lst,
  labelList& order,
  const Cmp& cmp
)
{
  if (lst.size() < 2) {
    order.clear();
    return;
  }
  sortedOrder(lst, order, cmp);
  label n = 0;
  for (label i = 0; i < order.size() - 1; ++i) {
    if (lst[order[i]] == lst[order[i+1]]) {
      order[n++] = order[i];
    }
  }
  order.setSize(n);
}


template<class T>
void mousse::uniqueOrder
(
  const UList<T>& lst,
  labelList& order
)
{
  uniqueOrder(lst, order, typename UList<T>::less(lst));
}


template<class T, class Cmp>
void mousse::uniqueOrder
(
  const UList<T>& lst,
  labelList& order,
  const Cmp& cmp
)
{
  sortedOrder(lst, order, cmp);
  if (order.size() > 1) {
    label n = 0;
    for (label i = 0; i < order.size() - 1; ++i) {
      if (lst[order[i]] != lst[order[i+1]]) {
        order[n++] = order[i];
      }
    }
    order[n++] = order[order.size()-1];
    order.setSize(n);
  }
}


template<class T, class ListType>
ListType mousse::subset
(
  const UList<T>& select,
  const T& value,
  const ListType& lst
)
{
  // select must at least cover the list range
  if (select.size() < lst.size()) {
    FATAL_ERROR_IN("subset(const UList<T>&, const T&, const ListType&)")
      << "select is of size " << select.size()
      << "; but it must index a list of size " << lst.size()
      << abort(FatalError);
  }
  ListType newLst{lst.size()};
  // ensure consistent addressable size {eg, DynamicList}
  newLst.setSize(lst.size());
  label nElem = 0;
  FOR_ALL(lst, elemI) {
    if (select[elemI] == value) {
      newLst[nElem++] = lst[elemI];
    }
  }
  newLst.setSize(nElem);
  return newLst;
}


template<class T, class ListType>
void mousse::inplaceSubset
(
  const UList<T>& select,
  const T& value,
  ListType& lst
)
{
  // select must at least cover the list range
  if (select.size() < lst.size()) {
    FATAL_ERROR_IN("inplaceSubset(const UList<T>&, const T&, ListType&)")
      << "select is of size " << select.size()
      << "; but it must index a list of size " << lst.size()
      << abort(FatalError);
  }
  label nElem = 0;
  FOR_ALL(lst, elemI) {
    if (select[elemI] == value) {
      if (nElem != elemI) {
        lst[nElem] = lst[elemI];
      }
      ++nElem;
    }
  }
  lst.setSize(nElem);
}


template<class BoolListType, class ListType>
ListType mousse::subset
(
  const BoolListType& select,
  const ListType& lst
)
{
  // select can have a different size
  // eg, when it is a PackedBoolList or a labelHashSet
  ListType newLst{lst.size()};
  // ensure consistent addressable size {eg, DynamicList}
  newLst.setSize(lst.size());
  label nElem = 0;
  FOR_ALL(lst, elemI) {
    if (select[elemI]) {
      newLst[nElem++] = lst[elemI];
    }
  }
  newLst.setSize(nElem);

  return newLst;
}


template<class BoolListType, class ListType>
void mousse::inplaceSubset
(
  const BoolListType& select,
  ListType& lst
)
{
  // select can have a different size
  // eg, when it is a PackedBoolList or a labelHashSet
  label nElem = 0;
  FOR_ALL(lst, elemI) {
    if (select[elemI]) {
      if (nElem != elemI) {
        lst[nElem] = lst[elemI];
      }
      ++nElem;
    }
  }
  lst.setSize(nElem);
}


// As clarification:
// coded as inversion from pointEdges to edges but completely general.
template<class InList, class OutList>
void mousse::invertManyToMany
(
  const label nEdges,
  const UList<InList>& pointEdges,
  List<OutList>& edges
)
{
  // Number of points per edge
  labelList nPointsPerEdge{nEdges, 0};
  FOR_ALL(pointEdges, pointI) {
    const InList& pEdges = pointEdges[pointI];
    FOR_ALL(pEdges, j) {
      nPointsPerEdge[pEdges[j]]++;
    }
  }
  // Size edges
  edges.setSize(nEdges);
  FOR_ALL(nPointsPerEdge, edgeI) {
    edges[edgeI].setSize(nPointsPerEdge[edgeI]);
  }
  nPointsPerEdge = 0;
  // Fill edges
  FOR_ALL(pointEdges, pointI) {
    const InList& pEdges = pointEdges[pointI];
    FOR_ALL(pEdges, j) {
      label edgeI = pEdges[j];
      edges[edgeI][nPointsPerEdge[edgeI]++] = pointI;
    }
  }
}


template<class ListType>
mousse::label mousse::findIndex
(
  const ListType& l,
  typename ListType::const_reference t,
  const label start
)
{
  label index = -1;
  for (label i = start; i < l.size(); i++) {
    if (l[i] == t) {
      index = i;
      break;
    }
  }
  return index;
}


template<class ListType>
mousse::labelList mousse::findIndices
(
  const ListType& l,
  typename ListType::const_reference t,
  const label start
)
{
  // Count occurrences
  label n = 0;
  for (label i = start; i < l.size(); i++) {
    if (l[i] == t) {
      n++;
    }
  }
  // Create and fill
  labelList indices(n);
  n = 0;
  for (label i = start; i < l.size(); i++) {
    if (l[i] == t) {
      indices[n++] = i;
    }
  }
  return indices;
}


template<class ListType>
void mousse::setValues
(
  ListType& l,
  const labelUList& indices,
  typename ListType::const_reference t
)
{
  FOR_ALL(indices, i) {
    l[indices[i]] = t;
  }
}


template<class ListType>
ListType mousse::createWithValues
(
  const label sz,
  const typename ListType::const_reference initValue,
  const labelUList& indices,
  typename ListType::const_reference setValue
)
{
  ListType l{sz, initValue};
  setValues(l, indices, setValue);
  return l;
}


template<class ListType>
mousse::label mousse::findMax(const ListType& l, const label start)
{
  if (start >= l.size()) {
    return -1;
  }
  label index = start;
  for (label i = start+1; i < l.size(); i++) {
    if (l[i] > l[index]) {
      index = i;
    }
  }
  return index;
}


template<class ListType>
mousse::label mousse::findMin(const ListType& l, const label start)
{
  if (start >= l.size()) {
    return -1;
  }
  label index = start;
  for (label i = start+1; i < l.size(); i++) {
    if (l[i] < l[index]) {
      index = i;
    }
  }
  return index;
}


template<class ListType>
mousse::label mousse::findSortedIndex
(
  const ListType& l,
  typename ListType::const_reference t,
  const label start
)
{
  if (start >= l.size()) {
    return -1;
  }
  label low = start;
  label high = l.size() - 1;
  while (low <= high) {
    label mid = (low + high)/2;
    if (t < l[mid]) {
      high = mid - 1;
    } else if (t > l[mid]) {
      low = mid + 1;
    } else {
      return mid;
    }
  }
  return -1;
}


template<class ListType, class BinaryOp>
mousse::label mousse::findLower
(
  const ListType& l,
  typename ListType::const_reference t,
  const label start,
  const BinaryOp& bop
)
{
  if (start >= l.size()) {
    return -1;
  }
  label low = start;
  label high = l.size() - 1;
  while ((high - low) > 1) {
    label mid = (low + high)/2;
    if (bop(l[mid], t)) {
      low = mid;
    } else {
      high = mid;
    }
  }
  if (bop(l[high], t)) {
    return high;
  } else {
    if (bop(l[low], t)) {
      return low;
    } else {
      return -1;
    }
  }
}


template<class ListType>
mousse::label mousse::findLower
(
  const ListType& l,
  typename ListType::const_reference t,
  const label start
)
{
  return findLower(l, t, start, lessOp<typename ListType::value_type>());
}


template<class Container, class T, int nRows>
mousse::List<Container> mousse::initList(const T elems[nRows])
{
  List<Container> lst{nRows};

  FOR_ALL(lst, rowI) {
    lst[rowI] = Container(elems[rowI]);
  }
  return lst;
}


template<class Container, class T, int nRows, int nColumns>
mousse::List<Container> mousse::initListList(const T elems[nRows][nColumns])
{
  List<Container> lst{nRows};
  Container cols{nColumns};

  FOR_ALL(lst, rowI) {
    FOR_ALL(cols, colI) {
      cols[colI] = elems[rowI][colI];
    }
    lst[rowI] = cols;
  }
  return lst;
}


template<class T>
void mousse::ListAppendEqOp<T>::operator()(List<T>& x, const List<T>& y) const
{
  if (y.size()) {
    if (x.size()) {
      label sz = x.size();
      x.setSize(sz + y.size());
      FOR_ALL(y, i) {
        x[sz++] = y[i];
      }
    } else {
      x = y;
    }
  }
}


template<class ListType>
ListType mousse::reverseList(const ListType& list)
{
  const label listSize = list.size();
  const label lastIndex = listSize - 1;
  ListType tmpList{listSize};

  FOR_ALL(tmpList, elemI) {
    tmpList[elemI] = list[lastIndex - elemI];
  }
  return tmpList;
}


template<class ListType>
void mousse::inplaceReverseList(ListType& list)
{
  const label listSize = list.size();
  const label lastIndex = listSize - 1;
  const label nIterations = listSize >> 1;
  label elemI = 0;

  while (elemI < nIterations) {
    Swap(list[elemI], list[lastIndex - elemI]);
    elemI++;
  }
}


template<class ListType>
ListType mousse::rotateList(const ListType& list, const label n)
{
  const label listSize = list.size();
  ListType tmpList{listSize};

  FOR_ALL(tmpList, elemI) {
    label index = (elemI - n) % listSize;
    if (index < 0) {
      index += listSize;
    }
    tmpList[elemI] = list[index];
  }

  return tmpList;
}


template<template<typename> class ListType, class DataType>
void mousse::inplaceRotateList(ListType<DataType>& list, label n)
{
  const label listSize = list.size();
  n = (listSize - n) % listSize;

  if (n < 0) {
    n += listSize;
  }

  SubList<DataType> firstHalf{list, n, 0};
  SubList<DataType> secondHalf{list, listSize - n, n};

  inplaceReverseList(firstHalf);
  inplaceReverseList(secondHalf);
  inplaceReverseList(list);
}
