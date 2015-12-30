// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::edgeFaceCirculator
// Description
//   Walks from starting face around edge.
//   Implicit description of edge:
//   - face
//   - index in face. edge is always between f[index] and f[index+1]
//   - direction (cell to walk into)
//   -# Use in-place: \n
//     \code
//       edgeFaceCirculator circ(..);
//       // Optionally rotate to beginning: circ.setCanonical();
//       // Walk
//       do
//       {
//         Info<< "face:" << circ.face() << endl;
//         ++circ;
//       }
//       while (circ != circ.end());
//     \endcode
//   -# Use like STL iterator: \n
//     \code
//       edgeFaceCirculator circ(..);
//       for
//       (
//         edgeFaceCirculator iter = circ.begin();
//         iter != circ.end();
//         ++iter
//       )
//       {
//         Info<< "face:" << iter.face() << endl;
//       }
//     \endcode
// SourceFiles
//   edge_face_circulator.cpp
#ifndef edge_face_circulator_hpp_
#define edge_face_circulator_hpp_
#include "face.hpp"
#include "list_ops.hpp"
namespace mousse
{
// Forward declaration of classes
class primitiveMesh;
class edgeFaceCirculator
{
  // Static data members
    //- End iterator
    static const edgeFaceCirculator endConstIter;
  // Private data
    //- Mesh
    const primitiveMesh& mesh_;
    //- Current face
    label faceLabel_;
    //- Current side of face
    bool ownerSide_;
    //- Edge (between index and index+1 on faces[faceLabel_]
    label index_;
    //- Is boundary edge?
    bool isBoundaryEdge_;
    //- Starting face so we know when to stop. Used when circulating over
    //  internal edges.
    label startFaceLabel_;
  // Private Member Functions
    //- Set to end() iterator
    inline void setEnd();
    //- Check and set faceLabel_ and ownerSide_
    inline void setFace(const label faceI, const label cellI);
    //- Set faceLabel_ to be the other face on the cell that uses the
    //  edge.
    inline void otherFace(const label cellI);
public:
  // Constructors
    //- Construct from components
    inline edgeFaceCirculator
    (
      const primitiveMesh& mesh,
      const label faceLabel,
      const bool ownerSide,
      const label index,
      const bool isBoundaryEdge
    );
    //- Construct as copy
    inline edgeFaceCirculator(const edgeFaceCirculator&);
  // Member Functions
    //- Helper: find index in face of edge or -1. Index is such that edge is
    //  between f[index] and f[index+1]
    inline static label getMinIndex
    (
      const face& f,
      const label v0,
      const label v1
    );
    inline label faceLabel() const;
    inline bool ownerSide() const;
    inline label index() const;
    //- Helper: get the neighbouring cell according to the ownerSide.
    //  Returns -1 if on neighbourside of boundary face.
    inline label cellLabel() const;
    //- Helper: return true if normal of generated face points along
    //  edge from v0 to v1. (v0 and v1 have to be on edge)
    inline bool sameOrder(const label v0, const label v1) const;
    //- Set edge to a unique state so different ones can be compared.
    //  Internal edge: minimum face index.
    //  Boundary edge: walk back until boundary face.
    inline void setCanonical();
  // Member Operators
    inline void operator=(const edgeFaceCirculator& iter);
    inline bool operator==(const edgeFaceCirculator& iter) const;
    inline bool operator!=(const edgeFaceCirculator& iter) const;
    //- Step to next face. Uses no edge addressing!
    inline edgeFaceCirculator& operator++();
    //- Iterator set to the beginning face. For internal edges this is
    //  the current face. For boundary edges this is the first boundary face
    //  reached from walking back (i.e. in opposite direction to ++)
    inline edgeFaceCirculator begin() const;
    inline edgeFaceCirculator cbegin() const;
    //- Iterator set to beyond the end of the walk.
    inline const edgeFaceCirculator& end() const;
    inline const edgeFaceCirculator& cend() const;
};
}  // namespace mousse
#include "edge_face_circulator_i.hpp"
#endif
