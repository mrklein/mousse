#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CV_CONTROLS_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CV_CONTROLS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cvControls
// Description
//   Controls for the conformalVoronoiMesh mesh generator

#include "dictionary.hpp"
#include "switch.hpp"
#include "vector.hpp"


namespace mousse {

// Forward declaration of classes
class conformalVoronoiMesh;


class cvControls
{
  // Private data
    //- Reference to the foamyHexMeshDict
    const dictionary& foamyHexMeshDict_;
    // Surface conformation controls
      //- Point pair spacing coefficient - fraction of the local target
      //  cell size
      scalar pointPairDistanceCoeff_;
      //- Distance along feature edge to place mixed feature point
      //  conformation groups - to be multiple of pointPairDistanceCoeff_.
      scalar mixedFeaturePointPPDistanceCoeff_;
      //- Distance to a feature point within which surface and edge
      //  conformation points are excluded - fraction of the local target
      //  cell size
      scalar featurePointExclusionDistanceCoeff_;
      //- Distance to an existing feature edge conformation location
      //  within which other edge conformation location are excluded -
      //  fraction of the local target cell size
      scalar featureEdgeExclusionDistanceCoeff_;
      //- Surface search distance coefficient - fraction of the local
      //  target cell size
      scalar surfaceSearchDistanceCoeff_;
      //- Maximum allowable protrusion through the surface before
      //  conformation points are added - fraction of the local target
      //  cell size
      scalar maxSurfaceProtrusionCoeff_;
      //- Maximum quadrant angle allowed at a concave edge before
      //  additional "mitering" lines are added
      scalar maxQuadAngle_;
      //- Now often to rebuild the surface conformation
      label surfaceConformationRebuildFrequency_;
      // Controls for feature point conformation
        //-
        Switch guardFeaturePoints_;
        //-
        Switch edgeAiming_;
        //-
        Switch snapFeaturePoints_;
        //-
        Switch circulateEdges_;
        //- Switch for using specialised feature points
        Switch specialiseFeaturePoints_;
      // Controls for coarse surface conformation
        //- Distance to an existing surface conformation point location
        //  within which other surface point locations are excluded
        //  - fraction of the local target cell size
        scalar surfacePtExclusionDistanceCoeff_;
        //- Distance to search for feature edges near to
        //  surface protrusions - fraction of the local target
        //  cell size.
        scalar edgeSearchDistCoeffSqr_;
        //- Proximity to a feature edge where a surface hit is
        //  not created, only the edge conformation is created
        //  - fraction of the local target cell size.
        scalar surfacePtReplaceDistCoeffSqr_;
        //- Maximum allowed number surface conformation iterations.
        label maxConformationIterations_;
        //- Termination criterion for conformation iterations.
        //  When the number of surface protrusions drops below this
        //  ratio of the initial number of protrusions.
        scalar iterationToInitialHitRatioLimit_;
  // Motion control controls
    // Cell size criteria
      //- Default cell size
      scalar defaultCellSize_;
      //- Minimum cell size
      scalar minimumCellSize_;
    //- Switch to control the output of obj files for debug
    Switch objOutput_;
    //- Switch to control the reporting of detailed timeChecks
    Switch timeChecks_;
    //- Switch to control the reporting of detailed vertex information
    Switch printVertexInfo_;
    //- Allowed relative load unbalance
    scalar maxLoadUnbalance_;
    //- Cosine of angle of alignment with required direction within which a
    //  face will be accepted for rotation
    scalar cosAlignmentAcceptanceAngle_;
    // Point insertion criteria
      //- Length between Delaunay vertices above which a new Dv should be
      //  inserted - fraction of the local target cell size
      scalar insertionDistCoeff_;
      //- Minimum dual face area corresponding to long Delaunay edge where
      //  a new Dv is to be inserted - fraction of the local target cell
      //  size
      scalar faceAreaRatioCoeff_;
      //- Cosine of the angle of alignment with the required direction
      //  within which a long De will be accepted for a new Dv insertion
      scalar cosInsertionAcceptanceAngle_;
    // Point removal criteria
      //- Length between Delaunay vertices below which a Dv should be
      //  removed - fraction of the local target cell size
      scalar removalDistCoeff_;
  // polyMesh filtering controls
    //- Activates the mesh edge filtering. On by default.
    Switch filterEdges_;
    //- Activates the mesh face filtering. Off by default.
    Switch filterFaces_;
    //- Write tet mesh at output time (it always writes the Voronoi)
    Switch writeTetDualMesh_;
    Switch writeCellShapeControlMesh_;
    Switch writeBackgroundMeshDecomposition_;
public:
 // Constructors
    //- Construct from references to conformalVoronoiMesh and dictionary
    cvControls
    (
      const dictionary& foamyHexMeshDict
    );
    //- Disallow default bitwise copy construct
    cvControls(const cvControls&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cvControls&) = delete;
  //- Destructor
  ~cvControls();
  // Member Functions
    // Access
      //- Return the foamyHexMeshDict
      inline const dictionary& foamyHexMeshDict() const;
      //- Return the pointPairDistanceCoeff
      inline scalar pointPairDistanceCoeff() const;
      //- Return the mixedFeaturePointPPDistanceCoeff
      inline scalar mixedFeaturePointPPDistanceCoeff() const;
      //- Return the featurePointExclusionDistanceCoeff
      inline scalar featurePointExclusionDistanceCoeff() const;
      //- Return the featureEdgeExclusionDistanceCoeff
      inline scalar featureEdgeExclusionDistanceCoeff() const;
      //-
      inline Switch guardFeaturePoints() const;
      //-
      inline Switch edgeAiming() const;
      //-
      inline Switch snapFeaturePoints() const;
      //-
      inline Switch circulateEdges() const;
      //- Return the surfacePtExclusionDistanceCoeff
      inline scalar surfacePtExclusionDistanceCoeff() const;
      //- Return whether to use specialised feature points
      inline Switch specialiseFeaturePoints() const;
      //- Return the surfaceSearchDistanceCoeff
      inline scalar surfaceSearchDistanceCoeff() const;
      //- Return the maxSurfaceProtrusionCoeff
      inline scalar maxSurfaceProtrusionCoeff() const;
      //- Return the maxQuadAngle
      inline scalar maxQuadAngle() const;
      //- Return the surfaceConformationRebuildFrequency
      inline label surfaceConformationRebuildFrequency() const;
      //- Return the edgeSearchDistCoeffSqr for conformation.
      scalar edgeSearchDistCoeffSqr() const;
      //- Return the surfacePtReplaceDistCoeffSqr for conformation.
      scalar surfacePtReplaceDistCoeffSqr() const;
      //- Return the maxConformationIterations
      label maxConformationIterations() const;
      //- Return the iterationToInitialHitRatioLimit
      scalar iterationToInitialHitRatioLimit() const;
      //- Return the defaultCellSize
      inline scalar defaultCellSize() const;
      //- Return the minimumCellSize
      inline scalar minimumCellSize() const;
      //- Return the objOutput Switch
      inline Switch objOutput() const;
      //- Return the timeChecks Switch
      inline Switch timeChecks() const;
      //- Return the printVertexInfo Switch
      inline Switch printVertexInfo() const;
      //- Return the maxLoadUnbalance
      inline scalar maxLoadUnbalance() const;
      //- Return the cosAlignmentAcceptanceAngle
      inline scalar cosAlignmentAcceptanceAngle() const;
      //- Return the aspectRatio
      inline scalar aspectRatio() const;
      //- Return the aspectRatioDirection
      inline const vector& aspectRatioDirection() const;
      //- Return the insertionDistCoeff
      inline scalar insertionDistCoeff() const;
      //- Return the faceAreaRatioCoeff
      inline scalar faceAreaRatioCoeff() const;
      //- Return the cosInsertionAcceptanceAngle
      inline scalar cosInsertionAcceptanceAngle() const;
      //- Return removalDistCoeff
      inline scalar removalDistCoeff() const;
      //- Filter edges at output time
      inline Switch filterEdges() const;
      //- Filter faces at output time
      inline Switch filterFaces() const;
      //- Write tetMesh at output time
      inline Switch writeTetDualMesh() const;
      //- Write cellShapeControlMesh at output time
      inline Switch writeCellShapeControlMesh() const;
      //- Write backgroundMeshDecomposition at output time
      inline Switch writeBackgroundMeshDecomposition() const;
};

}  // namespace mousse


// Member Functions 
inline const mousse::dictionary& mousse::cvControls::foamyHexMeshDict() const
{
  return foamyHexMeshDict_;
}


inline mousse::scalar mousse::cvControls::pointPairDistanceCoeff() const
{
  return pointPairDistanceCoeff_;
}


inline mousse::scalar mousse::cvControls::mixedFeaturePointPPDistanceCoeff() const
{
  return mixedFeaturePointPPDistanceCoeff_;
}


inline mousse::scalar mousse::cvControls::featurePointExclusionDistanceCoeff() const
{
  return featurePointExclusionDistanceCoeff_;
}


inline mousse::scalar mousse::cvControls::featureEdgeExclusionDistanceCoeff() const
{
  return featureEdgeExclusionDistanceCoeff_;
}


inline mousse::Switch mousse::cvControls::guardFeaturePoints() const
{
  return guardFeaturePoints_;
}


inline mousse::Switch mousse::cvControls::edgeAiming() const
{
  return edgeAiming_;
}


inline mousse::Switch mousse::cvControls::snapFeaturePoints() const
{
  return snapFeaturePoints_;
}


inline mousse::Switch mousse::cvControls::circulateEdges() const
{
  return circulateEdges_;
}


inline mousse::scalar mousse::cvControls::surfacePtExclusionDistanceCoeff() const
{
  return surfacePtExclusionDistanceCoeff_;
}


inline mousse::Switch mousse::cvControls::specialiseFeaturePoints() const
{
  return specialiseFeaturePoints_;
}


inline mousse::scalar mousse::cvControls::surfaceSearchDistanceCoeff() const
{
  return surfaceSearchDistanceCoeff_;
}


inline mousse::scalar mousse::cvControls::maxSurfaceProtrusionCoeff() const
{
  return maxSurfaceProtrusionCoeff_;
}


inline mousse::scalar mousse::cvControls::maxQuadAngle() const
{
  return maxQuadAngle_;
}


inline mousse::label mousse::cvControls::surfaceConformationRebuildFrequency() const
{
  return surfaceConformationRebuildFrequency_;
}


inline mousse::scalar mousse::cvControls::edgeSearchDistCoeffSqr() const
{
  return edgeSearchDistCoeffSqr_;
}


inline mousse::scalar mousse::cvControls::surfacePtReplaceDistCoeffSqr() const
{
  return surfacePtReplaceDistCoeffSqr_;
}


inline mousse::label mousse::cvControls::maxConformationIterations() const
{
  return maxConformationIterations_;
}


inline mousse::scalar mousse::cvControls::iterationToInitialHitRatioLimit() const
{
  return iterationToInitialHitRatioLimit_;
}


inline mousse::scalar mousse::cvControls::defaultCellSize() const
{
  return defaultCellSize_;
}


inline mousse::scalar mousse::cvControls::minimumCellSize() const
{
  return minimumCellSize_;
}


inline mousse::Switch mousse::cvControls::objOutput() const
{
  return objOutput_;
}


inline mousse::Switch mousse::cvControls::timeChecks() const
{
  return timeChecks_;
}


inline mousse::Switch mousse::cvControls::printVertexInfo() const
{
  return printVertexInfo_;
}


inline mousse::scalar mousse::cvControls::maxLoadUnbalance() const
{
  return maxLoadUnbalance_;
}


inline mousse::scalar mousse::cvControls::cosAlignmentAcceptanceAngle() const
{
  return cosAlignmentAcceptanceAngle_;
}


inline mousse::scalar mousse::cvControls::insertionDistCoeff() const
{
  return insertionDistCoeff_;
}


inline mousse::scalar mousse::cvControls::faceAreaRatioCoeff() const
{
  return faceAreaRatioCoeff_;
}


inline mousse::scalar mousse::cvControls::cosInsertionAcceptanceAngle() const
{
  return cosInsertionAcceptanceAngle_;
}


inline mousse::scalar mousse::cvControls::removalDistCoeff() const
{
  return removalDistCoeff_;
}


inline mousse::Switch mousse::cvControls::filterEdges() const
{
  return filterEdges_;
}


inline mousse::Switch mousse::cvControls::filterFaces() const
{
  return filterFaces_;
}


inline mousse::Switch mousse::cvControls::writeTetDualMesh() const
{
  return writeTetDualMesh_;
}


inline mousse::Switch mousse::cvControls::writeCellShapeControlMesh() const
{
  return writeCellShapeControlMesh_;
}


inline mousse::Switch mousse::cvControls::writeBackgroundMeshDecomposition() const
{
  return writeBackgroundMeshDecomposition_;
}

#endif

