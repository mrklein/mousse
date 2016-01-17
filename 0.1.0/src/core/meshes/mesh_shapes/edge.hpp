// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::edge
// Description
//   An edge is a list of two point labels. The functionality it provides
//   supports the discretisation on a 2-D flat mesh.

#ifndef edge_hpp_
#define edge_hpp_

#include "fixed_list.hpp"
#include "point_field.hpp"
#include "line_point_ref.hpp"
#include "iostreams.hpp"
#include "swap.hpp"
#include "uindirect_list.hpp"

namespace mousse
{
// Forward declaration of friend functions and operators
class edge;
inline bool operator==(const edge& a, const edge& b);
inline bool operator!=(const edge& a, const edge& b);
class edge
:
  public FixedList<label, 2>
{
public:
  // Static data members
    static const char* const typeName;
  // Constructors
    //- Null constructor for lists
    inline edge();
    //- Construct from components
    inline edge(const label a, const label b);
    //- Construct from FixedList
    inline edge(const FixedList<label, 2>&);
    //- Construct from Istream
    inline edge(Istream&);
  // Member Functions
    //- Return start vertex label
    inline label start() const;
    //- Return start vertex label
    inline label& start();
    //- Return end vertex label
    inline label end() const;
    //- Return end vertex label
    inline label& end();
    //- Given one vertex, return the other
    inline label otherVertex(const label a) const;
    //- Return common vertex
    inline label commonVertex(const edge& a) const;
    //- Flip the edge in-place.
    inline void flip();
    //- Return reverse edge
    inline edge reverseEdge() const;
    //- Return centre (centroid)
    inline point centre(const pointField&) const;
    //- Return the vector (end - start)
    inline vector vec(const pointField&) const;
    //- Return scalar magnitude
    inline scalar mag(const pointField&) const;
    //- Return edge line
    inline linePointRef line(const pointField&) const;
    //- Compare edges
    //  Returns:
    //  -  0: different
    //  - +1: identical
    //  - -1: same edge, but different orientation
    static inline int compare(const edge&, const edge&);
  // Friend Operators
    friend bool operator==(const edge& a, const edge& b);
    friend bool operator!=(const edge& a, const edge& b);
};
//- Hash specialization for hashing edges - a commutative hash value.
//  Hash incrementally.
template<>
inline unsigned Hash<edge>::operator()(const edge& e, unsigned seed) const
{
  unsigned val = seed;
  if (e[0] < e[1])
  {
    val = Hash<label>()(e[0], val);
    val = Hash<label>()(e[1], val);
  }
  else
  {
    val = Hash<label>()(e[1], val);
    val = Hash<label>()(e[0], val);
  }
  return val;
}
//- Hash specialization for hashing edges - a commutative hash value.
//  Hash incrementally.
template<>
inline unsigned Hash<edge>::operator()(const edge& e) const
{
  return Hash<edge>()(e, 0);
}
template<>
inline bool contiguous<edge>()  {return true;}
}  // namespace mousse

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

#endif
