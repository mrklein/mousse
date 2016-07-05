#ifndef CORE_MESHES_MESH_SHAPES_TET_CELL_HPP_
#define CORE_MESHES_MESH_SHAPES_TET_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetCell
// Description
//   A tetrahedral cell primitive.
//   It is important that the ordering of edges is the same for a tetrahedron
//   class, a tetrahedron cell shape model and a tetCell

#include "fixed_list.hpp"
#include "tri_face.hpp"
#include "edge.hpp"
#include "point_field.hpp"
#include "tetrahedron.hpp"
#include "iostreams.hpp"

 
namespace mousse {

class cellShape;


class tetCell
:
  public FixedList<label, 4>
{
public:
  // Constructors
    //- Construct null
    inline tetCell();
    //- Construct from four points
    inline tetCell
    (
      const label a,
      const label b,
      const label c,
      const label d
    );
    //- Construct from FixedList
    inline tetCell(const FixedList<label, 4>&);
    //- Construct from Istream
    inline tetCell(Istream&);
  // Member Functions
    // Access
      //- Return i-th face
      inline triFace face(const label faceI) const;
      //- Return first face adjacent to the given edge
      inline label edgeFace(const label edgeI) const;
      //- Return face adjacent to the given face sharing the same edge
      inline label edgeAdjacentFace
      (
        const label edgeI,
        const label faceI
      ) const;
      //- Return i-th edge
      inline edge tetEdge(const label edgeI) const;
    // Operations
      //- Return tet shape cell
      cellShape tetCellShape() const;
      //- Return the tetrahedron
      inline tetPointRef tet(const pointField&) const;
};

// Global Functions 
//- Data associated with the type are contiguous
template<> inline bool contiguous<tetCell>() {return true;}

}  // namespace mousse


// Constructors 
inline mousse::tetCell::tetCell()
{}


inline mousse::tetCell::tetCell
(
  const label a,
  const label b,
  const label c,
  const label d
)
{
  operator[](0) = a;
  operator[](1) = b;
  operator[](2) = c;
  operator[](3) = d;
}


inline mousse::tetCell::tetCell(const FixedList<label, 4>& lst)
:
  FixedList<label, 4>{lst}
{}


inline mousse::tetCell::tetCell(Istream& is)
:
  FixedList<label, 4>{is}
{}


// Member Functions 
inline mousse::triFace mousse::tetCell::face(const label faceI) const
{
  // Warning. Ordering of faces needs to be the same for a tetrahedron
  // class, a tetrahedron cell shape model and a tetCell
  static const label a[] = {1, 0, 0, 0};
  static const label b[] = {2, 3, 1, 2};
  static const label c[] = {3, 2, 3, 1};
#ifdef FULLDEBUG
  if (faceI >= 4)
  {
    FATAL_ERROR_IN("tetCell::face(const label faceI) const")
      << "index out of range 0 -> 3. faceI = " << faceI
      << abort(FatalError);
  }
#endif
  return // triFace
  {
    operator[](a[faceI]),
    operator[](b[faceI]),
    operator[](c[faceI])
  };
}


inline mousse::label mousse::tetCell::edgeFace(const label edgeI) const
{
  // Warning. Ordering of faces needs to be the same for a tetrahedron
  // class, a tetrahedron cell shape model and a tetCell
  //static const label edgeFaces[6] = {2, 1, 1, 0, 0, 0};
  static const label edgeFaces[6] = {2, 3, 1, 0, 0, 1};
#ifdef FULLDEBUG
  if (edgeI >= 6)
  {
    FATAL_ERROR_IN
    (
      "tetCell::edgeFace(const label edgeI)"
      "const"
    )
    << "edge index out of range 0 -> 5. edgeI = " << edgeI
    << abort(FatalError);
  }
#endif
  return edgeFaces[edgeI];
}


inline mousse::label mousse::tetCell::edgeAdjacentFace
(
  const label edgeI,
  const label faceI
) const
{
  // Warning. Ordering of faces needs to be the same for a tetrahedron
  // class, a tetrahedron cell shape model and a tetCell
  static const label adjacentFace[6][4] =
  {
    {-1, -1, 3, 2},
    {-1, 3, -1, 1},
    {-1, 2, 1, -1},
    {2, -1, 0, -1},
    {3, -1, -1, 0},
    {1, 0, -1, -1}
  };
#ifdef FULLDEBUG
  if (faceI >= 4)
  {
    FATAL_ERROR_IN
    (
      "tetCell::edgeAdjacentFace(const label edgeI, const label faceI)"
      "const"
    )
    << "face index out of range 0 -> 3. faceI = " << faceI
    << abort(FatalError);
  }
  if (edgeI >= 6)
  {
    FATAL_ERROR_IN
    (
      "tetCell::edgeAdjacentFace(const label edgeI, const label faceI)"
      "const"
    )
    << "edge index out of range 0 -> 5. edgeI = " << edgeI
    << abort(FatalError);
  }
#endif
  return adjacentFace[edgeI][faceI];
}


inline mousse::edge mousse::tetCell::tetEdge(const label edgeI) const
{
  // Warning. Ordering of edges needs to be the same for a tetrahedron
  // class, a tetrahedron cell shape model and a tetCell
  //
  static const label start[] = {0, 0, 0, 3, 1, 3};
  static const label end[] = {1, 2, 3, 1, 2, 2};
#ifdef FULLDEBUG
  if (edgeI >= 6)
  {
    FATAL_ERROR_IN("tetCell::tetEdge(const label edgeI) const")
      << "index out of range 0 -> 5. edgeI = " << edgeI
      << abort(FatalError);
  }
#endif
  return edge(operator[](start[edgeI]), operator[](end[edgeI]));
}


inline mousse::tetPointRef mousse::tetCell::tet(const pointField& points) const
{
  return // tetPointRef
  {
    points[operator[](0)],
    points[operator[](1)],
    points[operator[](2)],
    points[operator[](3)]
  };
}
#endif
