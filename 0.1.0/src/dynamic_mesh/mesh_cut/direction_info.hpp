#ifndef DYNAMIC_MESH_MESH_CUT_DIRECTIONS_DIRECTION_INFO_DIRECTION_INFO_HPP_
#define DYNAMIC_MESH_MESH_CUT_DIRECTIONS_DIRECTION_INFO_DIRECTION_INFO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::directionInfo
// Description
//   Holds direction in which to split cell (in fact a local coordinate axes).
//   Information is a label and a direction.
//   The direction is the normal
//   direction to cut in. The label's meaning depends on whether the info
//   is on a cell or on a face:
//     - in cell: edge that is being cut. (determines for hex how cut is)
//     - in face: local face point that is being cut or -1.
//       -# (-1)  : cut is tangential to plane
//       -# (>= 0): edge fp..fp+1 is cut
//       (has to be facepoint, not vertex since vertex not valid across
//       processors whereas f[0] should correspond to f[0] on other side)
//   The rule is that if the label is set (-1 or higher) it is used
//   (topological information only), otherwise the vector is used. This makes
//   sure that we use topological information as much as possible and so a
//   hex mesh is cut purely topologically. All other shapes are cut
//   geometrically.

#include "point.hpp"
#include "label_list.hpp"
#include "tensor.hpp"
#include "poly_mesh.hpp"
#include "mesh_tools.hpp"
#include "hex_matcher.hpp"


namespace mousse {

class polyPatch;
class polyMesh;
class primitiveMesh;
class edge;
class face;
class polyMesh;


class directionInfo
{
  // Private data
    // Either mesh edge or face point
    label index_;
    // Local n axis
    vector n_;
  // Private Member Functions
    //- Find edge among edgeLabels that uses v0 and v1
    static label findEdge
    (
      const primitiveMesh& mesh,
      const labelList& edgeLabels,
      const label v1,
      const label v0
    );
    //- Return 'lowest' of a,b in face of size.
    static label lowest
    (
      const label size,
      const label a,
      const label b
    );
public:
  // Static Functions
    //- Given edge on hex cell find corresponding edge on face. Is either
    //  index in face or -1 (cut tangential to face). Public since is
    //  needed to fill in seed faces in meshWave.
    static label edgeToFaceIndex
    (
      const primitiveMesh& mesh,
      const label cellI,
      const label faceI,
      const label edgeI
    );
  // Constructors
    //- Construct null
    inline directionInfo();
    //- Construct from components
    inline directionInfo
    (
      const label,
      const vector& n
    );
    //- Construct as copy
    inline directionInfo(const directionInfo&);
  // Member Functions
    // Access
      inline label index() const
      {
        return index_;
      }
      inline const vector& n() const
      {
        return n_;
      }
    // Needed by FaceCellWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Check for identical geometrical data. Used for cyclics checking.
      template<class TrackingData>
      inline bool sameGeometry
      (
        const polyMesh&,
        const directionInfo&,
        const scalar,
        TrackingData& td
      ) const;
      //- Convert any absolute coordinates into relative to (patch)face
      //  centre
      template<class TrackingData>
      inline void leaveDomain
      (
        const polyMesh&,
        const polyPatch&,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData& td
      );
      //- Reverse of leaveDomain
      template<class TrackingData>
      inline void enterDomain
      (
        const polyMesh&,
        const polyPatch&,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData& td
      );
      //- Apply rotation matrix to any coordinates
      template<class TrackingData>
      inline void transform
      (
        const polyMesh&,
        const tensor&,
        TrackingData& td
      );
      //- Influence of neighbouring face.
      template<class TrackingData>
      inline bool updateCell
      (
        const polyMesh&,
        const label thisCellI,
        const label neighbourFaceI,
        const directionInfo& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of neighbouring cell.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const label neighbourCellI,
        const directionInfo& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const directionInfo& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const directionInfo&, TrackingData& td) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const directionInfo&) const;
    inline bool operator!=(const directionInfo&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const directionInfo&);
    friend Istream& operator>>(Istream&, directionInfo&);
};


//- Data associated with directionInfo type are contiguous
template<>
inline bool contiguous<directionInfo>()
{
  return true;
}

}  // namespace mousse


// Constructors 
// Null constructor
inline mousse::directionInfo::directionInfo()
:
  index_{-3},
  n_{vector::zero}
{}


// Construct from components
inline mousse::directionInfo::directionInfo
(
  const label index,
  const vector& n
)
:
  index_{index},
  n_{n}
{}


// Construct as copy
inline mousse::directionInfo::directionInfo(const directionInfo& w2)
:
  index_{w2.index()},
  n_{w2.n()}
{}


// Member Functions 
template<class TrackingData>
inline bool mousse::directionInfo::valid(TrackingData&) const
{
  return index_ != -3;
}


// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::directionInfo::sameGeometry
(
  const polyMesh&,
  const directionInfo& /*w2*/,
  const scalar /*tol*/,
  TrackingData&
)
const
{
  return true;
}


// index_ is already offset in face
template<class TrackingData>
inline void mousse::directionInfo::leaveDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& /*faceCentre*/,
  TrackingData&
)
{}


// index_ is offset in face on other side. So reverse it here.
// (Note: f[0] on other domain is connected to f[0] in this domain,
//        f[1]      ,,                         f[size-1]   ,,
// etc.)
template<class TrackingData>
inline void mousse::directionInfo::enterDomain
(
  const polyMesh&,
  const polyPatch& patch,
  const label patchFaceI,
  const point& /*faceCentre*/,
  TrackingData&
)
{
  if (index_ >= 0) {
    const face& f = patch[patchFaceI];
    index_ = (f.size() - index_) % f.size();
  }
}


// No geometric data.
template<class TrackingData>
inline void mousse::directionInfo::transform
(
  const polyMesh&,
  const tensor& /*rotTensor*/,
  TrackingData&
)
{}


// Update this cell with neighbouring face information
template<class TrackingData>
inline bool mousse::directionInfo::updateCell
(
  const polyMesh& mesh,
  const label thisCellI,
  const label neighbourFaceI,
  const directionInfo& neighbourInfo,
  const scalar,       // tol
  TrackingData&
)
{
  if (index_ >= -2) {
    // Already determined.
    return false;
  }
  if (hexMatcher().isA(mesh, thisCellI)) {
    const face& f = mesh.faces()[neighbourFaceI];
    if (neighbourInfo.index() == -2) {
      // Geometric information from neighbour
      index_ = -2;
    } else if (neighbourInfo.index() == -1) {
      // Cut tangential to face. Take any edge connected to face
      // but not used in face.
      // Get first edge on face.
      label edgeI = mesh.faceEdges()[neighbourFaceI][0];
      const edge& e = mesh.edges()[edgeI];
      // Find face connected to face through edgeI and on same cell.
      label faceI =
        meshTools::otherFace(mesh, thisCellI, neighbourFaceI, edgeI);
      // Find edge on faceI which is connected to e.start() but not edgeI.
      index_ =
        meshTools::otherEdge(mesh, mesh.faceEdges()[faceI], edgeI, e.start());
    } else {
      // Index is a vertex on the face. Convert to mesh edge.
      // Get mesh edge between f[index_] and f[index_+1]
      label v0 = f[neighbourInfo.index()];
      label v1 = f[(neighbourInfo.index() + 1) % f.size()];
      index_ = findEdge(mesh, mesh.faceEdges()[neighbourFaceI], v0, v1);
    }
  } else {
    // Not a hex so mark this as geometric.
    index_ = -2;
  }
  n_ = neighbourInfo.n();
  return true;
}


// Update this face with neighbouring cell information
template<class TrackingData>
inline bool mousse::directionInfo::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const label neighbourCellI,
  const directionInfo& neighbourInfo,
  const scalar,   // tol
  TrackingData&
)
{
  // Handle special cases first
  if (index_ >= -2) {
    // Already determined
    return false;
  }
  // Handle normal cases where topological or geometrical info comes from
  // neighbouring cell
  if (neighbourInfo.index() >= 0) {
    // Neighbour has topological direction (and hence is hex). Find cut
    // edge on face.
    index_ =
      edgeToFaceIndex(mesh, neighbourCellI, thisFaceI, neighbourInfo.index());
  } else {
    // Neighbour has geometric information. Use.
    index_ = -2;
  }
  n_ = neighbourInfo.n();
  return true;
}


// Merge this with information on same face
template<class TrackingData>
inline bool mousse::directionInfo::updateFace
(
  const polyMesh&,
  const label,    // thisFaceI
  const directionInfo& neighbourInfo,
  const scalar,   // tol
  TrackingData&
)
{
  if (index_ >= -2) {
    // Already visited.
    return false;
  } else {
    index_ = neighbourInfo.index();
    n_ = neighbourInfo.n();
    return true;
  }
}


template<class TrackingData>
inline bool mousse::directionInfo::equal
(
  const directionInfo& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}


// Member Operators 
inline bool mousse::directionInfo::operator==(const mousse::directionInfo& rhs)
const
{
  return  index() == rhs.index() && n() == rhs.n();
}


inline bool mousse::directionInfo::operator!=(const mousse::directionInfo& rhs)
const
{
  return !(*this == rhs);
}

#endif
