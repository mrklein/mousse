// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_pairs.hpp"
// Private Member Functions 
template<class Triangulation>
inline mousse::Pair<mousse::labelPair>
mousse::pointPairs<Triangulation>::orderPointPair
(
  const labelPair& vA,
  const labelPair& vB
) const
{
  return
  (
    (vA < vB)
   ? Pair<labelPair>(vA, vB)
   : Pair<labelPair>(vB, vA)
  );
}
template<class Triangulation>
inline bool mousse::pointPairs<Triangulation>::findPointPair
(
  const labelPair& vA,
  const labelPair& vB
) const
{
  if (vA == vB)
  {
    return false;
  }
  else if (find(orderPointPair(vA, vB)) == end())
  {
    return false;
  }
  return true;
}
template<class Triangulation>
inline bool mousse::pointPairs<Triangulation>::insertPointPair
(
  const labelPair& vA,
  const labelPair& vB
)
{
  if (vA == vB)
  {
    return false;
  }
  return insert(orderPointPair(vA, vB));
}
// Constructors 
template<class Triangulation>
mousse::pointPairs<Triangulation>::pointPairs(const Triangulation& triangulation)
:
  ptPairTable(),
  triangulation_(triangulation)
{}
// Destructor 
template<class Triangulation>
mousse::pointPairs<Triangulation>::~pointPairs()
{}
// Member Functions 
template<class Triangulation>
inline bool mousse::pointPairs<Triangulation>::addPointPair
(
  const labelPair& vA,
  const labelPair& vB
)
{
  return insertPointPair(vA, vB);
}
template<class Triangulation>
inline bool mousse::pointPairs<Triangulation>::addPointPair
(
  const labelPair& master,
  const DynamicList<labelPair>& slaves
)
{
  FOR_ALL(slaves, sI)
  {
    const labelPair& slave = slaves[sI];
    addPointPair(master, slave);
  }
  return true;
}
template<class Triangulation>
inline bool mousse::pointPairs<Triangulation>::addPointPair
(
  const label vA,
  const label vB
)
{
  const label procNo = Pstream::myProcNo();
  labelPair a(vA, procNo);
  labelPair b(vB, procNo);
  return addPointPair(a, b);
}
template<class Triangulation>
inline bool mousse::pointPairs<Triangulation>::isPointPair
(
  const Vertex_handle& vA,
  const Vertex_handle& vB
) const
{
  if (vA->boundaryPoint() && vB->boundaryPoint())
  {
    labelPair a(vA->index(), vA->procIndex());
    labelPair b(vB->index(), vB->procIndex());
    return findPointPair(a, b);
  }
  return false;
}
template<class Triangulation>
inline bool mousse::pointPairs<Triangulation>::isPointPair
(
  const labelPair& vA,
  const labelPair& vB
) const
{
  return findPointPair(vA, vB);
}
template<class Triangulation>
void mousse::pointPairs<Triangulation>::reIndex(const Map<label>& oldToNewIndices)
{
  pointPairs<Triangulation> newTable(triangulation_);
  FOR_ALL_CONST_ITER(pointPairs, *this, iter)
  {
    Pair<labelPair> e = iter.key();
    labelPair& start = e.first();
    labelPair& end = e.second();
    bool insert = true;
    if (start.second() == Pstream::myProcNo())
    {
      Map<label>::const_iterator iter2 =
        oldToNewIndices.find(start.first());
      if (iter2 != oldToNewIndices.end())
      {
        if (iter2() != -1)
        {
          start.first() = iter2();
        }
        else
        {
          insert = false;
        }
      }
    }
    if (end.second() == Pstream::myProcNo())
    {
      Map<label>::const_iterator iter2 =
        oldToNewIndices.find(end.first());
      if (iter2 != oldToNewIndices.end())
      {
        if (iter2() != -1)
        {
          end.first() = iter2();
        }
        else
        {
          insert = false;
        }
      }
    }
    if (insert)
    {
      if (e.first() < e.second())
      {
        newTable.insert(e);
      }
      else if (e.first() > e.second())
      {
        newTable.insert(reverse(e));
      }
    }
  }
  this->transfer(newTable);
}
