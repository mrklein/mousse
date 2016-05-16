// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "feature_point_conformer.hpp"
#include "cv_controls.hpp"
#include "conformation_surfaces.hpp"
#include "conformal_voronoi_mesh.hpp"
#include "cell_shape_control.hpp"
#include "delaunay_mesh_tools.hpp"
#include "const_circulator.hpp"
#include "background_mesh_decomposition.hpp"
#include "auto_ptr.hpp"
#include "map_distribute.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(featurePointConformer, 0);

}

const mousse::scalar mousse::featurePointConformer::tolParallel = 1e-3;


// Private Member Functions 
mousse::vector mousse::featurePointConformer::sharedFaceNormal
(
  const extendedFeatureEdgeMesh& feMesh,
  const label edgeI,
  const label nextEdgeI
) const
{
  const labelList& edgeInormals = feMesh.edgeNormals()[edgeI];
  const labelList& nextEdgeInormals = feMesh.edgeNormals()[nextEdgeI];
  const vector& A1 = feMesh.normals()[edgeInormals[0]];
  const vector& A2 = feMesh.normals()[edgeInormals[1]];
  const vector& B1 = feMesh.normals()[nextEdgeInormals[0]];
  const vector& B2 = feMesh.normals()[nextEdgeInormals[1]];
  const scalar A1B1 = mag((A1 & B1) - 1.0);
  const scalar A1B2 = mag((A1 & B2) - 1.0);
  const scalar A2B1 = mag((A2 & B1) - 1.0);
  const scalar A2B2 = mag((A2 & B2) - 1.0);
  if (A1B1 < A1B2 && A1B1 < A2B1 && A1B1 < A2B2) {
    return 0.5*(A1 + B1);
  } else if (A1B2 < A1B1 && A1B2 < A2B1 && A1B2 < A2B2) {
    return 0.5*(A1 + B2);
  } else if (A2B1 < A1B1 && A2B1 < A1B2 && A2B1 < A2B2) {
    return 0.5*(A2 + B1);
  } else {
    return 0.5*(A2 + B2);
  }
}


mousse::label mousse::featurePointConformer::getSign
(
  const extendedFeatureEdgeMesh::edgeStatus eStatus
) const
{
  if (eStatus == extendedFeatureEdgeMesh::EXTERNAL) {
    return -1;
  } else if (eStatus == extendedFeatureEdgeMesh::INTERNAL) {
    return 1;
  }
  return 0;
}


void mousse::featurePointConformer::addMasterAndSlavePoints
(
  const DynamicList<mousse::point>& masterPoints,
  const DynamicList<mousse::indexedVertexEnum::vertexType>& masterPointsTypes,
  const Map<DynamicList<autoPtr<plane> > >& masterPointReflections,
  DynamicList<Vb>& pts,
  const label /*ptI*/
) const
{
  typedef DynamicList<autoPtr<plane>> planeDynList;
  typedef mousse::indexedVertexEnum::vertexType vertexType;
  FOR_ALL(masterPoints, pI) {
    // Append master to the list of points
    const mousse::point& masterPt = masterPoints[pI];
    const vertexType masterType = masterPointsTypes[pI];
    pts.append
    (
      Vb
      {
        masterPt,
        foamyHexMesh_.vertexCount() + pts.size(),
        masterType,
        Pstream::myProcNo()
      }
    );
    const label masterIndex = pts.last().index();
    const planeDynList& masterPointPlanes = masterPointReflections[pI];
    FOR_ALL(masterPointPlanes, planeI) {
      // Reflect master points in the planes and insert the slave points
      const plane& reflPlane = masterPointPlanes[planeI]();
      const mousse::point slavePt = reflPlane.mirror(masterPt);
      const vertexType slaveType =
      (
        masterType == Vb::vtInternalFeaturePoint
        ? Vb::vtExternalFeaturePoint
        : Vb::vtInternalFeaturePoint
      );
      pts.append
      (
        Vb
        {
          slavePt,
          foamyHexMesh_.vertexCount() + pts.size(),
          slaveType,
          Pstream::myProcNo()
        }
      );
      ftPtPairs_.addPointPair
      (
        masterIndex,
        pts.last().index()
      );
    }
  }
}


void mousse::featurePointConformer::createMasterAndSlavePoints
(
  const extendedFeatureEdgeMesh& feMesh,
  const label ptI,
  DynamicList<Vb>& pts
) const
{
  typedef DynamicList<autoPtr<plane>> planeDynList;
  typedef indexedVertexEnum::vertexType vertexType;
  typedef extendedFeatureEdgeMesh::edgeStatus edgeStatus;
  const mousse::point& featPt = feMesh.points()[ptI];
  if ((Pstream::parRun()
       && !foamyHexMesh_.decomposition().positionOnThisProcessor(featPt))
      || geometryToConformTo_.outside(featPt)) {
    return;
  }
  const scalar ppDist = foamyHexMesh_.pointPairDistance(featPt);
  // Maintain a list of master points and the planes to relect them in
  DynamicList<mousse::point> masterPoints;
  DynamicList<vertexType> masterPointsTypes;
  Map<planeDynList> masterPointReflections;
  const labelList& featPtEdges = feMesh.featurePointEdges()[ptI];
  pointFeatureEdgesTypes pointEdgeTypes(feMesh, ptI);
  const List<extendedFeatureEdgeMesh::edgeStatus> allEdStat =
    pointEdgeTypes.calcPointFeatureEdgesTypes();
  ConstCirculator<labelList> circ{featPtEdges};
  // Loop around the edges of the feature point
  if (circ.size()) {
    do {
      const edgeStatus eStatusCurr = feMesh.getEdgeStatus(circ());
      // Get the direction in which to move the point in relation to the
      // feature point
      label sign = getSign(eStatusCurr);
      const vector n = sharedFaceNormal(feMesh, circ(), circ.next());
      const vector pointMotionDirection = sign*0.5*ppDist*n;
      if (masterPoints.empty()) {
        // Initialise with the first master point
        mousse::point pt = featPt + pointMotionDirection;
        planeDynList firstPlane;
        firstPlane.append(autoPtr<plane>{new plane{featPt, n}});
        masterPoints.append(pt);
        masterPointsTypes.append
        (
          sign == 1
          ? Vb::vtExternalFeaturePoint // true
          : Vb::vtInternalFeaturePoint // false
        );
        masterPointReflections.insert
        (
          masterPoints.size() - 1,
          firstPlane
        );
      } else {
        // Just add this face contribution to the latest master point
        masterPoints.last() += pointMotionDirection;
        masterPointReflections[masterPoints.size() - 1].append
        (
          autoPtr<plane>{new plane{featPt, n}}
        );
      }
    } while (circ.circulate(CirculatorBase::CLOCKWISE));
  }
  addMasterAndSlavePoints
  (
    masterPoints,
    masterPointsTypes,
    masterPointReflections,
    pts,
    ptI
  );
}


void mousse::featurePointConformer::createMixedFeaturePoints
(
  DynamicList<Vb>& pts
) const
{
  if (foamyHexMeshControls_.mixedFeaturePointPPDistanceCoeff() < 0) {
    Info << nl
      << "Skipping specialised handling for mixed feature points" << endl;
    return;
  }
  const auto& feMeshes = geometryToConformTo_.features();
  FOR_ALL(feMeshes, i) {
    const auto& feMesh = feMeshes[i];
    const auto& pointsEdges = feMesh.pointEdges();
    const auto& points = feMesh.points();
    for (label ptI = feMesh.mixedStart();
         ptI < feMesh.nonFeatureStart();
         ptI++) {
      const auto& featPt = points[ptI];
      if (Pstream::parRun()
          && !foamyHexMesh_.decomposition().positionOnThisProcessor(featPt)) {
        continue;
      }
      const auto& pEds = pointsEdges[ptI];
      pointFeatureEdgesTypes pFEdgeTypes{feMesh, ptI};
      const auto allEdStat = pFEdgeTypes.calcPointFeatureEdgesTypes();
      bool specialisedSuccess = false;
      if (foamyHexMeshControls_.specialiseFeaturePoints()) {
        specialisedSuccess =
          createSpecialisedFeaturePoint
          (
            feMesh, pEds, pFEdgeTypes, allEdStat, ptI, pts
          );
      }
      if (!specialisedSuccess && foamyHexMeshControls_.edgeAiming()) {
        // Specialisations available for some mixed feature points.  For
        // non-specialised feature points, inserting mixed internal and
        // external edge groups at feature point.
        const auto& pt = points[ptI];
        const scalar edgeGroupDistance =
          foamyHexMesh_.mixedFeaturePointDistance(pt);
        FOR_ALL(pEds, e) {
          const label edgeI = pEds[e];
          const point edgePt =
            pt + edgeGroupDistance*feMesh.edgeDirection(edgeI, ptI);
          const pointIndexHit edgeHit{true, edgePt, edgeI};
          foamyHexMesh_.createEdgePointGroup(feMesh, edgeHit, pts);
        }
      }
    }
  }
}


void mousse::featurePointConformer::createFeaturePoints(DynamicList<Vb>& pts)
{
  const PtrList<extendedFeatureEdgeMesh>& feMeshes =
    geometryToConformTo_.features();
  FOR_ALL(feMeshes, i) {
    const extendedFeatureEdgeMesh& feMesh = feMeshes[i];
    for (label ptI = feMesh.convexStart();
         ptI < feMesh.mixedStart();
         ptI++) {
      createMasterAndSlavePoints(feMesh, ptI, pts);
    }
    if (!foamyHexMeshControls_.guardFeaturePoints())
      continue;
    for (label ptI = feMesh.mixedStart();
         ptI < feMesh.nonFeatureStart();
         ptI++) {
      pts.append
      (
        Vb
        {
          feMesh.points()[ptI],
          Vb::vtConstrained
        }
      );
    }
  }
}


// Constructors 
mousse::featurePointConformer::featurePointConformer
(
  const conformalVoronoiMesh& foamyHexMesh
)
:
  foamyHexMesh_{foamyHexMesh},
  foamyHexMeshControls_{foamyHexMesh.foamyHexMeshControls()},
  geometryToConformTo_{foamyHexMesh.geometryToConformTo()},
  featurePointVertices_{},
  ftPtPairs_{foamyHexMesh}
{
  Info << nl << "Conforming to feature points" << endl;
  Info << incrIndent
    << indent << "Circulating edges is: "
    << foamyHexMeshControls_.circulateEdges().asText() << nl
    << indent << "Guarding feature points is: "
    << foamyHexMeshControls_.guardFeaturePoints().asText() << nl
    << indent << "Snapping to feature points is: "
    << foamyHexMeshControls_.snapFeaturePoints().asText() << nl
    << indent << "Specialising feature points is: "
    << foamyHexMeshControls_.specialiseFeaturePoints().asText()
    << decrIndent
    << endl;
  DynamicList<Vb> pts;
  createFeaturePoints(pts);
  createMixedFeaturePoints(pts);
  // Points added using the createEdgePointGroup function will be labelled as
  // internal/external feature edges. Relabel them as feature points,
  // otherwise they are inserted as both feature points and surface points.
  FOR_ALL(pts, pI) {
    Vb& pt = pts[pI];
    if (pt.internalBoundaryPoint()) {
      pt.type() = Vb::vtInternalFeaturePoint;
    } else if (pt.externalBoundaryPoint()) {
      pt.type() = Vb::vtExternalFeaturePoint;
    }
  }
  if (foamyHexMeshControls_.objOutput()) {
    DelaunayMeshTools::writeOBJ("featureVertices.obj", pts);
  }
  featurePointVertices_.transfer(pts);
}


// Destructor 
mousse::featurePointConformer::~featurePointConformer()
{}


// Member Functions 
void mousse::featurePointConformer::distribute
(
  const backgroundMeshDecomposition& decomposition
)
{
  // Distribute points to their appropriate processor
  decomposition.distributePoints(featurePointVertices_);
  // Update the processor indices of the points to the new processor number
  FOR_ALL(featurePointVertices_, vI) {
    featurePointVertices_[vI].procIndex() = Pstream::myProcNo();
  }
  // Also update the feature point pairs
}


void mousse::featurePointConformer::reIndexPointPairs
(
  const Map<label>& oldToNewIndices
)
{
  FOR_ALL(featurePointVertices_, vI) {
    const label currentIndex = featurePointVertices_[vI].index();
    Map<label>::const_iterator newIndexIter =
      oldToNewIndices.find(currentIndex);
    if (newIndexIter != oldToNewIndices.end()) {
      featurePointVertices_[vI].index() = newIndexIter();
    }
  }
  ftPtPairs_.reIndex(oldToNewIndices);
}

