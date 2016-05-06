#ifndef MESH_AUTO_MESH_AUTO_HEX_MESH_EXTERNAL_DISPLACEMENT_MESH_MOVER_MEDIAL_AXIS_MESH_MOVER_HPP_
#define MESH_AUTO_MESH_AUTO_HEX_MESH_EXTERNAL_DISPLACEMENT_MESH_MOVER_MEDIAL_AXIS_MESH_MOVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::medialAxisMeshMover
// Description
//   Mesh motion solver that uses a medial axis algorithm to work
//   out a fraction between the (nearest point on a) moving surface
//   and the (nearest point on a) fixed surface.
//   This fraction is then used to scale the motion. Use
//   - fixedValue on all moving patches
//   - zeroFixedValue on stationary patches
//   - slip on all slipping patches

#include "external_displacement_mesh_mover.hpp"
#include "motion_smoother_algo.hpp"
#include "auto_layer_driver.hpp"


namespace mousse {

class pointData;


class medialAxisMeshMover
:
  public externalDisplacementMeshMover
{
  // Private data
    const labelList adaptPatchIDs_;
    autoPtr<indirectPrimitivePatch> adaptPatchPtr_;
    //- Scale factor for displacement
    pointScalarField scale_;
    //- Starting mesh position
    pointField oldPoints_;
    //- Mesh mover algorithm
    motionSmootherAlgo meshMover_;
  // Pre-calculated medial axis information
    //- Normal of nearest wall. Where this normal changes direction
    //  defines the medial axis
    pointVectorField dispVec_;
    //- Ratio of medial distance to wall distance.
    //  (1 at wall, 0 at medial axis)
    pointScalarField medialRatio_;
    //- Distance to nearest medial axis point
    pointScalarField medialDist_;
    //- Location on nearest medial axis point
    pointVectorField medialVec_;
  // Private Member Functions
    //- Extract fixed-value patchfields
    static labelList getFixedValueBCs(const pointVectorField&);
    //- Extract bc types. Replace fixedValue derivatives with fixedValue
    wordList getPatchFieldTypes(const pointVectorField& fld);
    //- Construct patch on selected patches
    static autoPtr<indirectPrimitivePatch> getPatch
    (
      const polyMesh&,
      const labelList&
    );
    // Calculation of medial axis information
      //- Smooth normals on patch
      void smoothPatchNormals
      (
        const label nSmoothDisp,
        const PackedBoolList& isMasterPoint,
        const PackedBoolList& isMasterEdge,
        pointField& normals
      ) const;
      //- Smooth normals on interior
      void smoothNormals
      (
        const label nSmoothDisp,
        const PackedBoolList& isMasterPoint,
        const PackedBoolList& isMasterEdge,
        const labelList& fixedPoints,
        pointVectorField& normals
      ) const;
      //- Is mesh edge on a cusp of displacement
      bool isMaxEdge
      (
        const List<pointData>& pointWallDist,
        const label edgeI,
        const scalar minCos
      ) const;
      //- Initialise medial axis. Uses pointDisplacement field only
      //  for boundary types, not values.
      void update(const dictionary&);
    // Calculation of mesh movement
      //- Unmark extrusion at given point
      static bool unmarkExtrusion
      (
        const label patchPointI,
        pointField& patchDisp,
        List<autoLayerDriver::extrudeMode>& extrudeStatus
      );
      //- Synchronise extrusion
      void syncPatchDisplacement
      (
        const scalarField& minThickness,
        pointField& patchDisp,
        List<autoLayerDriver::extrudeMode>& extrudeStatus
      ) const;
      void smoothLambdaMuDisplacement
      (
        const label nSmoothDisp,
        const PackedBoolList& isMasterPoint,
        const PackedBoolList& isMasterEdge,
        vectorField& displacement
      ) const;
      void minSmoothField
      (
        const label nSmoothDisp,
        const PackedBoolList& isMasterPoint,
        const PackedBoolList& isMasterEdge,
        const scalarField& fieldMin,
        scalarField& field
      ) const;
      //- Stop layer growth at feature edges
      void handleFeatureAngleLayerTerminations
      (
        const scalar minCos,
        const PackedBoolList& isMasterPoint,
        const labelList& meshEdges,
        List<autoLayerDriver::extrudeMode>& extrudeStatus,
        pointField& patchDisp,
        label& nPointCounter
      ) const;
      //- Find isolated islands (points, edges and faces and layer
      //  terminations) in the layer mesh and stop any layer growth
      //  at these points
      void findIsolatedRegions
      (
        const scalar minCosLayerTermination,
        const bool detectExtrusionIsland,
        const PackedBoolList& isMasterPoint,
        const PackedBoolList& isMasterEdge,
        const labelList& meshEdges,
        const scalarField& minThickness,
        List<autoLayerDriver::extrudeMode>& extrudeStatus,
        pointField& patchDisp
      ) const;
    //- Calculate desired displacement. Modifies per-patch displacement
    //  and calculates displacement for whole mesh
    //  (in pointDisplacement)
    void calculateDisplacement
    (
      const dictionary&,
      const scalarField& minThickness,
      List<autoLayerDriver::extrudeMode>& extrudeStatus,
      pointField& patchDisp
    );
    //- Move mesh according to calculated displacement
    bool shrinkMesh
    (
      const dictionary& meshQualityDict,
      const label nAllowableErrors,
      labelList& checkFaces
    );
public:
  //- Runtime type information
  TYPE_NAME("displacementMedialAxis");
  // Constructors
    //- Construct from dictionary and displacement field
    medialAxisMeshMover
    (
      const dictionary& dict,
      const List<labelPair>& baffles,
      pointVectorField& pointDisplacement
    );
    //- Disallow default bitwise copy construct
    medialAxisMeshMover(const medialAxisMeshMover&) = delete;
    //- Disallow default bitwise assignment
    medialAxisMeshMover& operator=(const medialAxisMeshMover&) = delete;
  // Destructor
    virtual ~medialAxisMeshMover();
  // Member Functions
    //- Move mesh using current pointDisplacement boundary values.
    //  Return true if succesful (errors on checkFaces less than
    //  allowable). Updates pointDisplacement.
    virtual bool move
    (
      const dictionary&,
      const label nAllowableErrors,
      labelList& checkFaces
    );
    //- Update local data for geometry changes
    virtual void movePoints(const pointField&);
    //-  Update local data for topology changes
    virtual void updateMesh(const mapPolyMesh&)
    {
      NOT_IMPLEMENTED
      (
        "medialAxisMeshMover::updateMesh"
        "(const mapPolyMesh&)"
      );
    }
};

}  // namespace mousse

#endif

