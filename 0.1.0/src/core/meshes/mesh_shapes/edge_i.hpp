// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iostreams.hpp"
#include "swap.hpp"
#include "uindirect_list.hpp"

// Static Member Functions
// return
//  -  0: different
//  - +1: identical
//  - -1: same edge, but different orientation
inline int mousse::edge::compare(const edge& a, const edge& b)
{
  if (a[0] == b[0] && a[1] == b[1])
  {
    return 1;
  }
  else if (a[0] == b[1] && a[1] == b[0])
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

// Constructors
inline mousse::edge::edge()
{}
inline mousse::edge::edge(const label a, const label b)
{
  start() = a;
  end() = b;
}
inline mousse::edge::edge(const FixedList<label, 2>& a)
{
  start() = a[0];
  end() = a[1];
}
inline mousse::edge::edge(Istream& is)
:
  FixedList<label, 2>(is)
{}

// Member Functions
inline mousse::label mousse::edge::start() const
{
  return operator[](0);
}
inline mousse::label& mousse::edge::start()
{
  return operator[](0);
}
inline mousse::label mousse::edge::end() const
{
  return operator[](1);
}
inline mousse::label& mousse::edge::end()
{
  return operator[](1);
}
inline mousse::label mousse::edge::otherVertex(const label a) const
{
  if (a == start())
  {
    return end();
  }
  else if (a == end())
  {
    return start();
  }
  else
  {
    // The given vertex is not on the edge in the first place.
    return -1;
  }
}
inline mousse::label mousse::edge::commonVertex(const edge& a) const
{
  if (start() == a.start() || start() == a.end())
  {
    return start();
  }
  else if (end() == a.start() || end() == a.end())
  {
    return end();
  }
  else
  {
    // No shared vertex.
    return -1;
  }
}
inline void mousse::edge::flip()
{
  Swap(operator[](0), operator[](1));
}
inline mousse::edge mousse::edge::reverseEdge() const
{
  return edge(end(), start());
}
inline mousse::point mousse::edge::centre(const pointField& p) const
{
  return 0.5*(p[start()] + p[end()]);
}
inline mousse::vector mousse::edge::vec(const pointField& p) const
{
  return p[end()] - p[start()];
}
inline mousse::scalar mousse::edge::mag(const pointField& p) const
{
  return ::mousse::mag(vec(p));
}
inline mousse::linePointRef mousse::edge::line(const pointField& p) const
{
  return linePointRef(p[start()], p[end()]);
}

// Friend Operators
inline bool mousse::operator==(const edge& a, const edge& b)
{
  return edge::compare(a,b) != 0;
}
inline bool mousse::operator!=(const edge& a, const edge& b)
{
  return edge::compare(a,b) == 0;
}
