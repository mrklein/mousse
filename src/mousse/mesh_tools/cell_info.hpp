#ifndef MESH_TOOLS_CELL_CLASSIFICATION_CELL_INFO_HPP_
#define MESH_TOOLS_CELL_CLASSIFICATION_CELL_INFO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellInfo
// Description
//   Holds information regarding type of cell. Used in inside/outside
//   determination in cellClassification.

#include "point.hpp"
#include "label.hpp"
#include "tensor.hpp"
#include "cell_classification.hpp"
#include "poly_mesh.hpp"


namespace mousse {

class polyPatch;
class polyMesh;


class cellInfo
{
  // Private data
    label type_;
  // Private Member Functions
    //- Update current cell/face type with neighbouring
    //  type. Return true if information needs to propagate,
    //  false otherwise.
    template<class TrackingData>
    inline bool update
    (
      const cellInfo& w2,
      const label thisFaceI,
      const label thisCellI,
      const label neighbourFaceI,
      const label neighbourCellI,
      TrackingData& td
    );
public:
  // Constructors
    //- Construct null
    inline cellInfo();
    //- Construct from cType
    inline cellInfo(const label);
    //- Construct as copy
    inline cellInfo(const cellInfo&);
  // Member Functions
    // Access
      inline label type() const
      {
        return type_;
      }
    // Needed by meshWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Check for identical geometrical data. Used for cyclics checking.
      template<class TrackingData>
      inline bool sameGeometry
      (
        const polyMesh&,
        const cellInfo&,
        const scalar,
        TrackingData& td
      ) const;
      //- Convert any absolute coordinates into relative to (patch)face
      //  centre
      template<class TrackingData>
      inline void leaveDomain
      (
        const polyMesh&,
        const polyPatch& patch,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData& td
      );
      //- Reverse of leaveDomain
      template<class TrackingData>
      inline void enterDomain
      (
        const polyMesh&,
        const polyPatch& patch,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData& td
      );
      //- Apply rotation matrix to any coordinates
      template<class TrackingData>
      inline void transform
      (
        const polyMesh&,
        const tensor& rotTensor,
        TrackingData& td
      );
      //- Influence of neighbouring face.
      template<class TrackingData>
      inline bool updateCell
      (
        const polyMesh&,
        const label thisCellI,
        const label neighbourFaceI,
        const cellInfo& neighbourInfo,
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
        const cellInfo& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const cellInfo& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const cellInfo&, TrackingData& td) const;
  // Member Operators
    //Note: Used to determine whether to call update.
    inline bool operator==(const cellInfo&) const;
    inline bool operator!=(const cellInfo&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const cellInfo&);
    friend Istream& operator>>(Istream&, cellInfo&);
};

//- Data associated with cellInfo type are contiguous
template<> inline bool contiguous<cellInfo>() { return true; }

}  // namespace mousse


// Private Member Functions 

// Update this with w2 information
template<class TrackingData>
inline bool mousse::cellInfo::update
(
  const cellInfo& w2,
  const label thisFaceI,
  const label thisCellI,
  const label neighbourFaceI,
  const label neighbourCellI,
  TrackingData& /*td*/
)
{
  if ((w2.type() == cellClassification::NOTSET)
      || (w2.type() == cellClassification::CUT)) {
    FATAL_ERROR_IN("cellInfo::update(const cellInfo&)")
      << "Problem: trying to propagate NOTSET or CUT type:" << w2.type()
      << " into cell/face with type:" << type() << endl
      << "thisFaceI:" << thisFaceI
      << "  thisCellI:" << thisCellI
      << "  neighbourFaceI:" << neighbourFaceI
      << "  neighbourCellI:" << neighbourCellI
      << abort(FatalError);
    return false;
  }
  if (type() == cellClassification::NOTSET) {
    type_ = w2.type();
    return true;
  }
  if (type() == cellClassification::CUT) {
    // Reached boundary. Stop.
    return false;
  }
  if (type() == w2.type()) {
    // Should never happen; already checked in meshWave
    return false;
  }
  // Two conflicting types
  FATAL_ERROR_IN("cellInfo::update(const cellInfo&)")
    << "Problem: trying to propagate conflicting types:" << w2.type()
    << " into cell/face with type:" << type() << endl
    << "thisFaceI:" << thisFaceI
    << "  thisCellI:" << thisCellI
    << "  neighbourFaceI:" << neighbourFaceI
    << "  neighbourCellI:" << neighbourCellI
    << abort(FatalError);
  return false;
}


// Constructors 
// Null constructor
inline mousse::cellInfo::cellInfo()
:
  type_{cellClassification::NOTSET}
{}


// Construct from components
inline mousse::cellInfo::cellInfo(const label type)
:
  type_{type}
{}


// Construct as copy
inline mousse::cellInfo::cellInfo(const cellInfo& w2)
:
  type_{w2.type()}
{}


// Member Functions 
template<class TrackingData>
inline bool mousse::cellInfo::valid(TrackingData&) const
{
  return type_ != cellClassification::NOTSET;
}


// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::cellInfo::sameGeometry
(
  const polyMesh&,
  const cellInfo& /*w2*/,
  const scalar /*tol*/,
  TrackingData&
)
const
{
  return true;
}


// No geometric data.
template<class TrackingData>
inline void mousse::cellInfo::leaveDomain
(
  const polyMesh&,
  const polyPatch& /*patch*/,
  const label /*patchFaceI*/,
  const point& /*faceCentre*/,
  TrackingData&
)
{}


// No geometric data.
template<class TrackingData>
inline void mousse::cellInfo::transform
(
  const polyMesh&,
  const tensor& /*rotTensor*/,
  TrackingData&
)
{}


// No geometric data.
template<class TrackingData>
inline void mousse::cellInfo::enterDomain
(
  const polyMesh&,
  const polyPatch& /*patch*/,
  const label /*patchFaceI*/,
  const point& /*faceCentre*/,
  TrackingData&
)
{}


// Update this with neighbour information
template<class TrackingData>
inline bool mousse::cellInfo::updateCell
(
  const polyMesh&,
  const label thisCellI,
  const label neighbourFaceI,
  const cellInfo& neighbourInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  return update
  (
    neighbourInfo,
    -1,
    thisCellI,
    neighbourFaceI,
    -1,
    td
  );
}


// Update this with neighbour information
template<class TrackingData>
inline bool mousse::cellInfo::updateFace
(
  const polyMesh&,
  const label thisFaceI,
  const label neighbourCellI,
  const cellInfo& neighbourInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  return update
  (
    neighbourInfo,
    thisFaceI,
    -1,
    -1,
    neighbourCellI,
    td
  );
}


// Update this with neighbour information
template<class TrackingData>
inline bool mousse::cellInfo::updateFace
(
  const polyMesh&,
  const label thisFaceI,
  const cellInfo& neighbourInfo,
  const scalar /*tol*/,
  TrackingData& td
)
{
  return update
  (
    neighbourInfo,
    thisFaceI,
    -1,
    -1,
    -1,
    td
  );
}


template<class TrackingData>
inline bool mousse::cellInfo::equal
(
  const cellInfo& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}


// Member Operators 
inline bool mousse::cellInfo::operator==(const mousse::cellInfo& rhs) const
{
  return type() == rhs.type();
}


inline bool mousse::cellInfo::operator!=(const mousse::cellInfo& rhs) const
{
  return !(*this == rhs);
}

#endif
