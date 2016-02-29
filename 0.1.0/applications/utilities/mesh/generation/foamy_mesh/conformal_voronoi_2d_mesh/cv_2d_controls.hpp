#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_2D_MESH_CV_2D_CONTROLS_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_2D_MESH_CV_2D_CONTROLS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
// Class
//   mousse::cv2DControls
// Description
//   Controls for the 2D CV mesh generator.
// SourceFiles
//   cv_2d_controls.cpp


#include "switch.hpp"
#include "dictionary.hpp"
#include "bound_box.hpp"

namespace mousse
{
class cv2DControls
{
  // Private data
    //- Description of data_
    // const dictionary& dict_;
    const dictionary& motionControl_;
    const dictionary& conformationControl_;
public:
    // Controls
      //- Minimum cell size below which protusions through the surface are
      //  not split
      scalar minCellSize_;
      //- Square of minCellSize
      scalar minCellSize2_;
      //- Maximum quadrant angle allowed at a concave corner before
      //  additional "mitering" lines are added
      scalar maxQuadAngle_;
      //- Near-wall region where cells are aligned with the wall
      scalar nearWallAlignedDist_;
      //- Square of nearWallAlignedDist
      scalar nearWallAlignedDist2_;
      //- Insert near-boundary point mirror or point-pairs
      Switch insertSurfaceNearestPointPairs_;
      //- Mirror near-boundary points rather than insert point-pairs
      Switch mirrorPoints_;
      //- Insert point-pairs vor dual-cell vertices very near the surface
      Switch insertSurfaceNearPointPairs_;
      Switch objOutput_;
      Switch meshedSurfaceOutput_;
      Switch randomiseInitialGrid_;
      scalar randomPerturbation_;
      label maxBoundaryConformingIter_;
    // Tolerances
      //- Maximum cartesian span of the geometry
      scalar span_;
      //- Square of span
      scalar span2_;
      //- Minumum edge-length of the cell size below which protusions
      //  through the surface are not split
      scalar minEdgeLen_;
      //- Square of minEdgeLen
      scalar minEdgeLen2_;
      //- Maximum notch size below which protusions into the surface are
      //  not filled
      scalar maxNotchLen_;
      //- Square of maxNotchLen
      scalar maxNotchLen2_;
      //- The minimum distance alowed between a dual-cell vertex
      // and the surface before a point-pair is introduced
      scalar minNearPointDist_;
      //- Square of minNearPoint
      scalar minNearPointDist2_;
      //- Distance between boundary conforming point-pairs
      scalar ppDist_;
      //- Square of ppDist
      scalar ppDist2_;
  // Constructors
    cv2DControls
    (
      const dictionary& controlDict,
      const boundBox& bb
    );
    //- Disallow default bitwise copy construct
    cv2DControls(const cv2DControls&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cv2DControls&) = delete;
  //- Destructor
  ~cv2DControls();
  // Member Functions
    // Access
      //- Return the minimum cell size
      inline scalar minCellSize() const;
      //- Return the square of the minimum cell size
      inline scalar minCellSize2() const;
      //- Return the maximum quadrant angle
      inline scalar maxQuadAngle() const;
      //- Return number of layers to align with the nearest wall
      inline scalar nearWallAlignedDist() const;
      //- Return square of nearWallAlignedDist
      inline scalar nearWallAlignedDist2() const;
      //- Return insertSurfaceNearestPointPairs Switch
      inline Switch insertSurfaceNearestPointPairs() const;
      //- Return mirrorPoints Switch
      inline Switch mirrorPoints() const;
      //- Return insertSurfaceNearPointPairs Switch
      inline Switch insertSurfaceNearPointPairs() const;
      //- Return the objOutput Switch
      inline Switch objOutput() const;
      //- Return the meshedSurfaceOutput Switch
      inline Switch meshedSurfaceOutput() const;
      //- Return the randomise initial point layout Switch
      inline Switch randomiseInitialGrid() const;
      //- Return the random perturbation factor
      inline scalar randomPerturbation() const;
      //- Return the maximum number of boundary conformation iterations
      inline label maxBoundaryConformingIter() const;
      //- Return the span
      inline scalar span() const;
      //- Return the span squared
      inline scalar span2() const;
      //- Return the minEdgeLen
      inline scalar minEdgeLen() const;
      //- Return the minEdgeLen squared
      inline scalar minEdgeLen2() const;
      //- Return the maxNotchLen
      inline scalar maxNotchLen() const;
      //- Return the maxNotchLen squared
      inline scalar maxNotchLen2() const;
      //- Return the minNearPointDist
      inline scalar minNearPointDist() const;
      //- Return the minNearPointDist squared
      inline scalar minNearPointDist2() const;
      //- Return the ppDist
      inline scalar ppDist() const;
    // Write
      //- Write controls to output stream.
      void write(Ostream& os) const;
      //- Ostream Operator
      friend Ostream& operator<<
      (
        Ostream& os,
        const cv2DControls& s
      );
};
}  // namespace mousse

inline mousse::scalar mousse::cv2DControls::minCellSize() const
{
  return minCellSize_;
}
inline mousse::scalar mousse::cv2DControls::minCellSize2() const
{
  return minCellSize2_;
}
inline mousse::scalar mousse::cv2DControls::maxQuadAngle() const
{
  return maxQuadAngle_;
}
inline mousse::scalar mousse::cv2DControls::nearWallAlignedDist() const
{
  return nearWallAlignedDist_;
}
inline mousse::scalar mousse::cv2DControls::nearWallAlignedDist2() const
{
  return nearWallAlignedDist2_;
}
inline mousse::Switch mousse::cv2DControls::insertSurfaceNearestPointPairs() const
{
  return insertSurfaceNearestPointPairs_;
}
inline mousse::Switch mousse::cv2DControls::mirrorPoints() const
{
  return mirrorPoints_;
}
inline mousse::Switch mousse::cv2DControls::insertSurfaceNearPointPairs() const
{
  return insertSurfaceNearPointPairs_;
}
inline mousse::Switch mousse::cv2DControls::objOutput() const
{
  return objOutput_;
}
inline mousse::Switch mousse::cv2DControls::meshedSurfaceOutput() const
{
  return meshedSurfaceOutput_;
}
inline mousse::Switch mousse::cv2DControls::randomiseInitialGrid() const
{
  return randomiseInitialGrid_;
}
inline mousse::scalar mousse::cv2DControls::randomPerturbation() const
{
  return randomPerturbation_;
}
inline mousse::label mousse::cv2DControls::maxBoundaryConformingIter() const
{
  return maxBoundaryConformingIter_;
}
inline mousse::scalar mousse::cv2DControls::span() const
{
  return span_;
}
inline mousse::scalar mousse::cv2DControls::span2() const
{
  return span2_;
}
inline mousse::scalar mousse::cv2DControls::minEdgeLen() const
{
  return minEdgeLen_;
}
inline mousse::scalar mousse::cv2DControls::minEdgeLen2() const
{
  return minEdgeLen2_;
}
inline mousse::scalar mousse::cv2DControls::maxNotchLen() const
{
  return maxNotchLen_;
}
inline mousse::scalar mousse::cv2DControls::maxNotchLen2() const
{
  return maxNotchLen2_;
}
inline mousse::scalar mousse::cv2DControls::minNearPointDist() const
{
  return minNearPointDist_;
}
inline mousse::scalar mousse::cv2DControls::minNearPointDist2() const
{
  return minNearPointDist2_;
}
inline mousse::scalar mousse::cv2DControls::ppDist() const
{
  return ppDist_;
}
#endif
