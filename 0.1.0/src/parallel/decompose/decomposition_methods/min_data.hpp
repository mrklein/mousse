// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::minData
// Description
//   For use with FaceCellWave. Transports minimum passive data
// SourceFiles
//   min_data_i.hpp
#ifndef min_data_hpp_
#define min_data_hpp_
#include "point.hpp"
#include "tensor.hpp"
namespace mousse
{
class polyPatch;
class polyMesh;
class minData
{
  // Private data
    //- Starting data
    label data_;
public:
  // Constructors
    //- Construct null
    inline minData();
    //- Construct from count
    inline minData(const label data);
  // Member Functions
    // Access
      inline label data() const
      {
        return data_;
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
        const minData&,
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
        const minData& neighbourInfo,
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
        const minData& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const minData& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const minData&, TrackingData& td) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const minData&) const;
    inline bool operator!=(const minData&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const minData&);
    friend Istream& operator>>(Istream&, minData&);
};
//- Data associated with minData type are contiguous
template<>
inline bool contiguous<minData>()
{
  return true;
}
}  // namespace mousse
#include "min_data_i.hpp"
#endif
