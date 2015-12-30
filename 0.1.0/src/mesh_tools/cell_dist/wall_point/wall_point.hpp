// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallPoint
// Description
//   Holds information regarding nearest wall point. Used in wall distance
//   calculation.
// SourceFiles
//   wall_point_i.hpp
//   wall_point.cpp
#ifndef wall_point_hpp_
#define wall_point_hpp_
#include "point.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
namespace mousse
{
// Forward declaration of classes
class polyPatch;
class polyMesh;
class wallPoint;
// Forward declaration of friend functions and operators
Ostream& operator<<(Ostream&, const wallPoint&);
Istream& operator>>(Istream&, wallPoint&);
class wallPoint
{
  // Private data
    //- Position of nearest wall center
    point origin_;
    //- Normal distance (squared) from cellcenter to origin
    scalar distSqr_;
  // Private Member Functions
    //- Evaluate distance to point. Update distSqr, origin from whomever
    //  is nearer pt. Return true if w2 is closer to point,
    //  false otherwise.
    template<class TrackingData>
    inline bool update
    (
      const point&,
      const wallPoint& w2,
      const scalar tol,
      TrackingData& td
    );
public:
  // Constructors
    //- Construct null
    inline wallPoint();
    //- Construct from origin, distance
    inline wallPoint(const point& origin, const scalar distSqr);
    //- Construct as copy
    inline wallPoint(const wallPoint&);
  // Member Functions
    // Access
      inline const point& origin() const;
      inline point& origin();
      inline scalar distSqr() const;
      inline scalar& distSqr();
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
        const wallPoint&,
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
        const wallPoint& neighbourInfo,
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
        const wallPoint& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const wallPoint& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const wallPoint&, TrackingData& td) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const wallPoint&) const;
    inline bool operator!=(const wallPoint&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const wallPoint&);
    friend Istream& operator>>(Istream&, wallPoint&);
};
//- Data associated with wallPoint type are contiguous
template<>
inline bool contiguous<wallPoint>()
{
  return true;
}
}  // namespace mousse
#include "wall_point_i.hpp"
#endif
