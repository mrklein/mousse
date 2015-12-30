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
//   wall_point_y_plus_i.hpp
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
#include "wall_point_y_plus_i.hpp"
#endif
