// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetCell
// Description
//   A tetrahedral cell primitive.
//   It is important that the ordering of edges is the same for a tetrahedron
//   class, a tetrahedron cell shape model and a tetCell
// SourceFiles
//   tet_cell.cpp
//   tet_cell_i.hpp
#ifndef tet_cell_hpp_
#define tet_cell_hpp_
#include "fixed_list.hpp"
#include "tri_face.hpp"
#include "edge.hpp"
#include "point_field.hpp"
#include "tetrahedron.hpp"
namespace mousse
{
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
template<>
inline bool contiguous<tetCell>() {return true;}
}  // namespace mousse
#include "tet_cell_i.hpp"
#endif
