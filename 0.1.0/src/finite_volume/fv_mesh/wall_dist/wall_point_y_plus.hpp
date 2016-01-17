// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallPointYPlus
// Description
//   Holds information (coordinate and yStar) regarding nearest wall point.
//   Used in VanDriest wall damping where the interest is in y+ but only
//   needs to be calculated up to e.g. y+ < 200. In all other cells/faces
//   the damping function becomes 1, since y gets initialized to GREAT and
//   yStar to 1.
//   Note: should feed the additional argument (yPlusCutoff) through as a
//   template argument into FaceCellWave
// SourceFiles
//   wall_point_y_plus.cpp
#ifndef wall_point_y_plus_hpp_
#define wall_point_y_plus_hpp_
#include "wall_point_data.hpp"
namespace mousse
{
class wallPointYPlus
:
  public wallPointData<scalar>
{
  // Private Member Functions
    //- Evaluate distance to point. Update distSqr, origin from whomever
    //  is nearer pt. Return true if w2 is closer to point,
    //  false otherwise.
    template<class TrackingData>
    inline bool update
    (
      const point&,
      const wallPointYPlus& w2,
      const scalar tol,
      TrackingData& td
    );
public:
  // Static data members
    //- cut-off value for y+
    static scalar yPlusCutOff;
  // Constructors
    //- Construct null
    inline wallPointYPlus();
    //- Construct from origin, yStar, distance
    inline wallPointYPlus
    (
      const point& origin,
      const scalar yStar,
      const scalar distSqr
    );
  // Member Functions
    // Needed by FaceCellWave
      //- Influence of neighbouring face.
      //  Calls update(...) with cellCentre of cellI
      template<class TrackingData>
      inline bool updateCell
      (
        const polyMesh& mesh,
        const label thisCellI,
        const label neighbourFaceI,
        const wallPointYPlus& neighbourWallInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of neighbouring cell.
      //  Calls update(...) with faceCentre of faceI
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh& mesh,
        const label thisFaceI,
        const label neighbourCellI,
        const wallPointYPlus& neighbourWallInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face.
      //  Merge new and old info.
      //  Calls update(...) with faceCentre of faceI
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh& mesh,
        const label thisFaceI,
        const wallPointYPlus& neighbourWallInfo,
        const scalar tol,
        TrackingData& td
      );
};
//- Data associated with pointEdgePoint type as contiguous as underlying type
template<>
inline bool contiguous<wallPointYPlus>()
{
  return contiguous<wallPointData<scalar> >();
}
}  // namespace mousse

namespace mousse
{
// Private Member Functions 
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool wallPointYPlus::update
(
  const point& pt,
  const wallPointYPlus& w2,
  const scalar tol,
  TrackingData& td
)
{
  scalar dist2 = magSqr(pt - w2.origin());
  if (valid(td))
  {
    scalar diff = distSqr() - dist2;
    if (diff < 0)
    {
      // already nearer to pt
      return false;
    }
    if ((diff < SMALL) || ((distSqr() > SMALL) && (diff/distSqr() < tol)))
    {
      // don't propagate small changes
      return false;
    }
  }
  // Either *this is not yet valid or w2 is closer
  {
    // only propagate if interesting (i.e. y+ < 100)
    scalar yPlus = mousse::sqrt(dist2)/w2.data();
    if (yPlus < yPlusCutOff)
    {
      // update with new values
      distSqr() = dist2;
      origin() = w2.origin();
      data() = w2.data();
      return true;
    }
    else
    {
      return false;
    }
  }
}
// Constructors 
// Null constructor
inline wallPointYPlus::wallPointYPlus()
:
  wallPointData<scalar>{}
{
  // Important: value of yStar where meshWave does not come.
  data() = 1.0;
}
// Construct from components
inline wallPointYPlus::wallPointYPlus
(
  const point& origin,
  const scalar yStar,
  const scalar distSqr
)
:
  wallPointData<scalar>{origin, yStar, distSqr}
{}
// Member Functions 
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool wallPointYPlus::updateCell
(
  const polyMesh& mesh,
  const label thisCellI,
  const label /*neighbourFaceI*/,
  const wallPointYPlus& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  const vectorField& cellCentres = mesh.primitiveMesh::cellCentres();
  return update
  (
    cellCentres[thisCellI],
    neighbourWallInfo,
    tol,
    td
  );
}
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool wallPointYPlus::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const label /*neighbourCellI*/,
  const wallPointYPlus& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  const vectorField& faceCentres = mesh.faceCentres();
  return update
  (
    faceCentres[thisFaceI],
    neighbourWallInfo,
    tol,
    td
  );
}
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool wallPointYPlus::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const wallPointYPlus& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  const vectorField& faceCentres = mesh.faceCentres();
  return update
  (
    faceCentres[thisFaceI],
    neighbourWallInfo,
    tol,
    td
  );
}
}  // namespace mousse
#endif
