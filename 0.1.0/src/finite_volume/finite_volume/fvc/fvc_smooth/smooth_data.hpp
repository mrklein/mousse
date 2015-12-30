// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::smoothData
// Description
//   Helper class used by the fvc::smooth and fvc::spread functions.
// SourceFiles
//   smooth_data.hpp
//   smooth_data_i.hpp
#ifndef smooth_data_hpp_
#define smooth_data_hpp_
namespace mousse
{
class smoothData
{
public:
  //- Class used to pass additional data in
  class trackData
  {
  public:
    //- Cut off distance
    scalar maxRatio;
  };
private:
  scalar value_;
  // Private Member Functions
    //- Update - gets information from neighbouring face/cell and
    //  uses this to update itself (if necessary) and return true
    template<class TrackingData>
    inline bool update
    (
      const smoothData& svf,
      const scalar scale,
      const scalar tol,
      TrackingData& td
    );
public:
  // Constructors
    //- Construct null
    inline smoothData();
    //- Construct from inverse field value
    inline smoothData(const scalar value);
  // Member Functions
    // Access
      //- Return value
      scalar value() const
      {
        return value_;
      }
    // Needed by FaceCellWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Check for identical geometrical data
      //  Used for cyclics checking
      template<class TrackingData>
      inline bool sameGeometry
      (
        const polyMesh&,
        const smoothData&,
        const scalar,
        TrackingData& td
      ) const;
      //- Convert any absolute coordinates into relative to
      //  (patch)face centre
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
      //- Influence of neighbouring face
      template<class TrackingData>
      inline bool updateCell
      (
        const polyMesh&,
        const label thisCellI,
        const label neighbourFaceI,
        const smoothData& svf,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of neighbouring cell
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const label neighbourCellI,
        const smoothData& svf,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const smoothData& svf,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const smoothData&, TrackingData& td) const;
    // Member Operators
      inline void operator=(const scalar value);
      // Needed for List IO
      inline bool operator==(const smoothData&) const;
      inline bool operator!=(const smoothData&) const;
    // IOstream Operators
      friend Ostream& operator<<
      (
        Ostream& os,
        const smoothData& svf
      )
      {
        return os  << svf.value_;
      }
      friend Istream& operator>>(Istream& is, smoothData& svf)
      {
        return is  >> svf.value_;
      }
};
//- Data associated with smoothData type are contiguous
template<>
inline bool contiguous<smoothData>()
{
  return true;
}
}  // namespace mousse
#include "smooth_data_i.hpp"
#endif
