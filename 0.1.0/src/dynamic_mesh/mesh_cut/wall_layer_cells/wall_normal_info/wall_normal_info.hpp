// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallNormalInfo
// Description
//   Holds information regarding nearest wall point.
//   Used in wall refinement.
// SourceFiles
//   wall_normal_info_i.hpp
//   wall_normal_info.cpp
#ifndef wall_normal_info_hpp_
#define wall_normal_info_hpp_
#include "point.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
namespace mousse
{
// Forward declaration of classes
class polyPatch;
class polyMesh;
class wallNormalInfo
{
  // Private data
    //- Normal at nearest wall point
    vector normal_;
  // Private Member Functions
    //- Evaluate distance to point. Update normal_
    template<class TrackingData>
    inline bool update(const wallNormalInfo& w2, TrackingData& td);
public:
  // Constructors
    //- Construct null
    inline wallNormalInfo();
    //- Construct from normal
    inline wallNormalInfo(const vector& normal);
    //- Construct as copy
    inline wallNormalInfo(const wallNormalInfo&);
  // Member Functions
    // Access
      inline const vector& normal() const;
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
        const wallNormalInfo&,
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
        const wallNormalInfo& neighbourInfo,
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
        const wallNormalInfo& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const wallNormalInfo& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const wallNormalInfo&, TrackingData& td) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const wallNormalInfo&) const;
    inline bool operator!=(const wallNormalInfo&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const wallNormalInfo&);
    friend Istream& operator>>(Istream&, wallNormalInfo&);
};
//- Data associated with wallNormalInfo type are contiguous
template<>
inline bool contiguous<wallNormalInfo>()
{
  return true;
}
}  // namespace mousse
#include "wall_normal_info_i.hpp"
#endif
