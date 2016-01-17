// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellMatcher
// Description
//   Base class for cellshape matchers (hexMatch, prismMatch, etc.). These are
//   classes which given a mesh and cell number find out the orientation of
//   the cellShape and construct cell-vertex to mesh-vertex mapping and
//   cell-face to mesh-face mapping.
//   For example,
//   \verbatim
//     hexMatcher hex(mesh);
//     cellShape shape;
//     ..
//     bool isHex = hex.match(cellI, shape);
//   \endverbatim
//   Now shape is set to the correct Hex cellShape (if \a isHex is true)
//   Alternatively there is direct access to the vertex and face mapping:
//   \verbatim
//     const labelList& hexVertLabels = hex.vertLabels();
//     const labelList& hexFaceLabels = hex.faceLabels();
//   \endverbatim
//   Now
//    - \c hexVertLabels[n] is vertex label of hex vertex n
//    - \c hexFaceLabels[n] is face   label of hex vertex n
//   Process of cellShape recognition consists of following steps:
//   - renumber vertices of cell to local vertex numbers
//   - construct (local to cell) addressing edge-to-faces
//   - construct (local to cell) addressing vertex and face to index in face
//   - find most unique face shape (e.g. triangle for prism)
//   - walk (following either vertices in face or jumping from face to other
//    face) to other faces and checking face sizes.
//   - if necessary try other rotations of this face
//    (only necessary for wedge, tet-wedge)
//   - if necessary try other faces which most unique face shape
//    (never necessary for hex degenerates)
//   The whole calculation is done such that no lists are allocated during
//   cell checking. E.g. localFaces_ are always sized to hold max. number
//   of possible face vertices and a separate list is filled which holds
//   the actusl face sizes.
//   For now all hex-degenerates implemented. Numbering taken from picture in
//   demoGuide.
// SourceFiles
//   cell_matcher.cpp

#ifndef cell_matcher_hpp_
#define cell_matcher_hpp_

#include "label_list.hpp"
#include "face_list.hpp"
#include "bool_list.hpp"
#include "map.hpp"
#include "primitive_mesh.hpp"
#include "cell_modeller.hpp"
#include "cell_model.hpp"

namespace mousse
{

// Forward declaration of classes
class primitiveMesh;
class cell;
class cellShape;
class cellModel;

class cellMatcher
{
protected:

  // Static functions

    //- Given start and end of edge generate unique key
    inline static label edgeKey
    (
      const label numVert,
      const label v0,
      const label v1
    );

    //- Step along face either in righthand or lefthand direction
    inline static label nextVert(const label, const label, const bool);

  // Protected data

    // Map from mesh to local vertex numbering
    Map<label> localPoint_;

    //- Faces using local vertex numbering
    faceList localFaces_;

    //- Number of vertices per face in localFaces_
    labelList faceSize_;

    //- Map from local to mesh vertex numbering
    labelList pointMap_;

    //- Map from local to mesh face numbering
    labelList faceMap_;

    //- Map from 'edge' to neighbouring faces
    labelList edgeFaces_;

    //- pointFaceIndex[localVertI][localFaceI] is index in localFace
    //  where localVertI is.
    labelListList pointFaceIndex_;

    //- After matching: holds mesh vertices in cellmodel order
    labelList vertLabels_;

    //- After matching: holds mesh faces in cellmodel order
    labelList faceLabels_;

    //- CellModel name
    const word cellModelName_;
    mutable const cellModel* cellModelPtr_;

  // Protected Member Functions

    //- Calculates localFaces. Returns number of local vertices (or -1
    //  if more than vertPerCell).
    label calcLocalFaces(const faceList& faces, const labelList& myFaces);

    //- Fill edge (start, end) to face number
    void calcEdgeAddressing(const label numVert);

    //- Fill vertex/face to index in face data structure
    void calcPointFaceIndex();

    //- Given start,end of edge lookup both faces sharing it and return
    //  face != localFaceI
    label otherFace
    (
      const label numVert,
      const label v0,
      const label v1,
      const label localFaceI
    ) const;

public:
  // Constructors
    //- Construct given mesh and shape factors
    cellMatcher
    (
      const label vertPerCell,
      const label facePerCell,
      const label maxVertPerFace,
      const word& cellModelName
    );

    //- Disallow default bitwise copy construct and assignment
    cellMatcher(const cellMatcher&) = delete;

    cellMatcher& operator=(const cellMatcher&) = delete;

  //- Destructor
  virtual ~cellMatcher()
  {}

  // Member Functions

    // Access

      inline const Map<label>& localPoint() const;

      inline const faceList& localFaces() const;

      inline const labelList& faceSize() const;

      inline const labelList& pointMap() const;

      inline const labelList& faceMap() const;

      inline const labelList& edgeFaces() const;

      inline const labelListList& pointFaceIndex() const;

      inline const labelList& vertLabels() const;

      inline const labelList& faceLabels() const;

      inline const cellModel& model() const;

    // Write

      void write(Ostream& os) const;

    // Cell shape dependent

      virtual label nVertPerCell() const = 0;

      virtual label nFacePerCell() const = 0;

      virtual label nMaxVertPerFace() const = 0;

      //- Hash value of all face sizes of this shape. Can be used for
      //  quick initial recognition.
      virtual label faceHashValue() const = 0;

      //- Check whether number of face sizes match the shape.
      virtual bool faceSizeMatch(const faceList&, const labelList&)
        const = 0;

      //- Low level shape recognition. Return true if matches.
      //  Works in detection mode only (checkOnly=true) or in exact
      //  matching. Returns true and sets vertLabels_.
      //  Needs faces, faceOwner of all faces in 'mesh' and cell number
      //  and labels of faces for this cell.
      //  cellI only used in combination with faceOwner to detect owner
      //  status.
      virtual bool matchShape
      (
        const bool checkOnly,
        const faceList& faces,
        const labelList& faceOwner,
        const label cellI,
        const labelList& myFaces
      ) = 0;

      //- Exact match. Uses faceSizeMatch.
      //  Returns true if cell matches shape exactly.
      virtual bool isA(const primitiveMesh& mesh, const label cellI) = 0;

      //- Exact match given all the faces forming a cell. No checks
      //  on whether faces match up and form a closed shape.
      virtual bool isA(const faceList&) = 0;

      //- Like isA but also constructs a cellShape (if shape matches)
      virtual bool matches
      (
        const primitiveMesh& mesh,
        const label cellI,
        cellShape& shape
      ) = 0;
};

}  // namespace mousse

// Member Functions 
inline const mousse::Map<mousse::label>& mousse::cellMatcher::localPoint() const
{
  return localPoint_;
}

inline const mousse::faceList& mousse::cellMatcher::localFaces() const
{
  return localFaces_;
}

inline const mousse::labelList& mousse::cellMatcher::faceSize() const
{
  return faceSize_;
}

inline const mousse::labelList& mousse::cellMatcher::pointMap() const
{
  return pointMap_;
}

inline const mousse::labelList& mousse::cellMatcher::faceMap() const
{
  return faceMap_;
}

inline const mousse::labelList& mousse::cellMatcher::edgeFaces() const
{
  return edgeFaces_;
}

inline const mousse::labelListList& mousse::cellMatcher::pointFaceIndex() const
{
  return pointFaceIndex_;
}

inline const mousse::labelList& mousse::cellMatcher::vertLabels() const
{
  return vertLabels_;
}

inline const mousse::labelList& mousse::cellMatcher::faceLabels() const
{
  return faceLabels_;
}

inline const mousse::cellModel& mousse::cellMatcher::model() const
{
  if (cellModelPtr_ == NULL)
  {
    cellModelPtr_ = cellModeller::lookup(cellModelName_);
  }
  return *cellModelPtr_;
}

// Key into edgeFaces_. key and key+1 are the entries for edge going from
// v0 to v1
inline mousse::label mousse::cellMatcher::edgeKey
(
  const label numVert,
  const label v0,
  const label v1
)
{
  return 2*(v0*numVert + v1);
}

// Walk along face consistent with face orientation
inline mousse::label mousse::cellMatcher::nextVert
(
  const label localVertI,
  const label size,
  const bool rightHand
)
{
  if (rightHand)
  {
    // face oriented acc. to righthand rule
    return (localVertI + 1) % size;
  }
  else
  {
    // face oriented acc. to lefthand rule
    return (size + localVertI - 1) % size;
  }
}

#endif
