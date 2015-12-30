// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellInfo
// Description
//   Holds information regarding type of cell. Used in inside/outside
//   determination in cellClassification.
// SourceFiles
//   cell_info_i.hpp
//   cell_info.cpp
#ifndef cell_info_hpp_
#define cell_info_hpp_
#include "point.hpp"
#include "label.hpp"
#include "tensor.hpp"
namespace mousse
{
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
template<>
inline bool contiguous<cellInfo>()
{
  return true;
}
}  // namespace mousse
#include "cell_info_i.hpp"
#endif
