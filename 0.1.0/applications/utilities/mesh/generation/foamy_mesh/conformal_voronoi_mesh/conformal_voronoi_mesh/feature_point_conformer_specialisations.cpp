// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "feature_point_conformer.hpp"
#include "vector_tools.hpp"
#include "point_feature_edges_types.hpp"
#include "conformal_voronoi_mesh.hpp"
#include "point_conversion.hpp"


using namespace mousse::vectorTools;


// Private Member Functions 
bool mousse::featurePointConformer::createSpecialisedFeaturePoint
(
  const extendedFeatureEdgeMesh& feMesh,
  const labelList& pEds,
  const pointFeatureEdgesTypes& pFEdgesTypes,
  const List<extendedFeatureEdgeMesh::edgeStatus>& allEdStat,
  const label ptI,
  DynamicList<Vb>& pts
) const
{
  if (!pFEdgesTypes.found(extendedFeatureEdgeMesh::EXTERNAL)
      || !pFEdgesTypes.found(extendedFeatureEdgeMesh::INTERNAL)) {
    return false;
  }
  if (pFEdgesTypes[extendedFeatureEdgeMesh::EXTERNAL] == 2
      && pFEdgesTypes[extendedFeatureEdgeMesh::INTERNAL] == 1
      && pEds.size() == 3) {
    if (debug)
      Info << "nExternal == 2 && nInternal == 1" << endl;
    const mousse::point& featPt = feMesh.points()[ptI];
    if (Pstream::parRun()
        && !foamyHexMesh_.decomposition().positionOnThisProcessor(featPt)) {
      return false;
    }
    label nVert = foamyHexMesh_.number_of_vertices();
    const label initialNumOfPoints = pts.size();
    const scalar ppDist = foamyHexMesh_.pointPairDistance(featPt);
    const vectorField& normals = feMesh.normals();
    const labelListList& edgeNormals = feMesh.edgeNormals();
    label concaveEdgeI = -1;
    labelList convexEdgesI{2, label{-1}};
    label nConvex = 0;
    FOR_ALL(pEds, i) {
      const extendedFeatureEdgeMesh::edgeStatus& eS = allEdStat[i];
      if (eS == extendedFeatureEdgeMesh::INTERNAL) {
        concaveEdgeI = pEds[i];
      } else if (eS == extendedFeatureEdgeMesh::EXTERNAL) {
        convexEdgesI[nConvex++] = pEds[i];
      } else if (eS == extendedFeatureEdgeMesh::FLAT) {
        WARNING_IN("mousse::conformalVoronoiMesh::"
          "createSpecialisedFeaturePoint")
          << "Edge " << eS << " is flat"
          << endl;
      } else {
        FATAL_ERROR_IN("mousse::conformalVoronoiMesh::"
          "createSpecialisedFeaturePoint")
          << "Edge " << eS << " not concave/convex"
          << exit(FatalError);
      }
    }
    const vector& concaveEdgePlaneANormal =
      normals[edgeNormals[concaveEdgeI][0]];
    const vector& concaveEdgePlaneBNormal =
      normals[edgeNormals[concaveEdgeI][1]];
    // Intersect planes parallel to the concave edge planes offset
    // by ppDist and the plane defined by featPt and the edge vector.
    plane planeA
    {
      featPt + ppDist*concaveEdgePlaneANormal,
      concaveEdgePlaneANormal
    };
    plane planeB
    {
      featPt + ppDist*concaveEdgePlaneBNormal,
      concaveEdgePlaneBNormal
    };
    const vector& concaveEdgeDir =
      feMesh.edgeDirection(concaveEdgeI, ptI);
    // Todo,needed later but want to get rid of this.
    const mousse::point concaveEdgeLocalFeatPt =
      featPt + ppDist*concaveEdgeDir;
    // Finding the nearest point on the intersecting line to the edge
    // point. Floating point errors often occur using planePlaneIntersect
    plane planeF{concaveEdgeLocalFeatPt, concaveEdgeDir};
    const mousse::point concaveEdgeExternalPt =
      planeF.planePlaneIntersect(planeA, planeB);
    // Redefine planes to be on the feature surfaces to project through
    planeA = plane{featPt, concaveEdgePlaneANormal};
    planeB = plane{featPt, concaveEdgePlaneBNormal};
    const mousse::point internalPtA =
      concaveEdgeExternalPt
      - 2.0*planeA.distance(concaveEdgeExternalPt)*concaveEdgePlaneANormal;
    pts.append
    (
      Vb
      {
        internalPtA,
        foamyHexMesh_.vertexCount() + pts.size(),
        Vb::vtInternalFeaturePoint,
        Pstream::myProcNo()
      }
    );
    const label internalPtAIndex(pts.last().index());
    const mousse::point internalPtB =
      concaveEdgeExternalPt
      - 2.0*planeB.distance(concaveEdgeExternalPt)*concaveEdgePlaneBNormal;
    pts.append
    (
      Vb
      {
        internalPtB,
        foamyHexMesh_.vertexCount() + pts.size(),
        Vb::vtInternalFeaturePoint,
        Pstream::myProcNo()
      }
    );
    const label internalPtBIndex{pts.last().index()};
    // Add the external points
    mousse::point externalPtD;
    mousse::point externalPtE;
    vector convexEdgePlaneCNormal{vector::zero};
    vector convexEdgePlaneDNormal{vector::zero};
    const labelList& concaveEdgeNormals = edgeNormals[concaveEdgeI];
    const labelList& convexEdgeANormals = edgeNormals[convexEdgesI[0]];
    const labelList& convexEdgeBNormals = edgeNormals[convexEdgesI[1]];
    FOR_ALL(concaveEdgeNormals, edgeNormalI) {
      bool convexEdgeA = false;
      bool convexEdgeB = false;
      FOR_ALL(convexEdgeANormals, edgeAnormalI) {
        const vector& concaveNormal
          = normals[concaveEdgeNormals[edgeNormalI]];
        const vector& convexNormal
          = normals[convexEdgeANormals[edgeAnormalI]];
        if (debug) {
          Info << "Angle between vectors = "
            << degAngleBetween(concaveNormal, convexNormal) << endl;
        }
        // Need a looser tolerance, because sometimes adjacent triangles
        // on the same surface will be slightly out of alignment.
        if (areParallel(concaveNormal, convexNormal, tolParallel)) {
          convexEdgeA = true;
        }
      }
      FOR_ALL(convexEdgeBNormals, edgeBnormalI) {
        const vector& concaveNormal
          = normals[concaveEdgeNormals[edgeNormalI]];
        const vector& convexNormal
          = normals[convexEdgeBNormals[edgeBnormalI]];
        if (debug) {
          Info << "Angle between vectors = "
            << degAngleBetween(concaveNormal, convexNormal) << endl;
        }
        // Need a looser tolerance, because sometimes adjacent triangles
        // on the same surface will be slightly out of alignment.
        if (areParallel(concaveNormal, convexNormal, tolParallel)) {
          convexEdgeB = true;
        }
      }
      if ((convexEdgeA && convexEdgeB) || (!convexEdgeA && !convexEdgeB)) {
        WARNING_IN
        (
          "mousse::conformalVoronoiMesh"
          "::createSpecialisedFeaturePoint"
        )
        << "Both or neither of the convex edges share the concave "
        << "edge's normal."
        << " convexEdgeA = " << convexEdgeA
        << " convexEdgeB = " << convexEdgeB
        << endl;
        // Remove points that have just been added before returning
        for (label i = 0; i < 2; ++i) {
          pts.remove();
          nVert--;
        }
        return false;
      }
      if (convexEdgeA) {
        FOR_ALL(convexEdgeANormals, edgeAnormalI) {
          const vector& concaveNormal =
            normals[concaveEdgeNormals[edgeNormalI]];
          const vector& convexNormal =
            normals[convexEdgeANormals[edgeAnormalI]];
          if (!areParallel(concaveNormal, convexNormal, tolParallel)) {
            convexEdgePlaneCNormal = convexNormal;
            plane planeC{featPt, convexEdgePlaneCNormal};
            externalPtD =
              internalPtA
              + 2.0*planeC.distance(internalPtA)*convexEdgePlaneCNormal;
            pts.append
            (
              Vb
              {
                externalPtD,
                foamyHexMesh_.vertexCount() + pts.size(),
                Vb::vtExternalFeaturePoint,
                Pstream::myProcNo()
              }
            );
            ftPtPairs_.addPointPair
            (
              internalPtAIndex,
              pts.last().index()
            );
          }
        }
      }
      if (convexEdgeB) {
        FOR_ALL(convexEdgeBNormals, edgeBnormalI) {
          const vector& concaveNormal =
            normals[concaveEdgeNormals[edgeNormalI]];
          const vector& convexNormal =
            normals[convexEdgeBNormals[edgeBnormalI]];
          if (!areParallel(concaveNormal, convexNormal, tolParallel)) {
            convexEdgePlaneDNormal = convexNormal;
            plane planeD{featPt, convexEdgePlaneDNormal};
            externalPtE =
              internalPtB
              + 2.0*planeD.distance(internalPtB)*convexEdgePlaneDNormal;
            pts.append
            (
              Vb
              {
                externalPtE,
                foamyHexMesh_.vertexCount() + pts.size(),
                Vb::vtExternalFeaturePoint,
                Pstream::myProcNo()
              }
            );
            ftPtPairs_.addPointPair
            (
              internalPtBIndex,
              pts.last().index()
            );
          }
        }
      }
    }
    pts.append
    (
      Vb
      {
        concaveEdgeExternalPt,
        foamyHexMesh_.vertexCount() + pts.size(),
        Vb::vtExternalFeaturePoint,
        Pstream::myProcNo()
      }
    );
    ftPtPairs_.addPointPair
    (
      internalPtBIndex,
      pts.last().index()
    );
    ftPtPairs_.addPointPair
    (
      internalPtAIndex,
      pts.last().index()
    );
    const label concaveEdgeExternalPtIndex{pts.last().index()};
    const scalar totalAngle =
      radToDeg
      (
        constant::mathematical::pi
        + radAngleBetween(concaveEdgePlaneANormal, concaveEdgePlaneBNormal)
      );
    if (totalAngle > foamyHexMeshControls_.maxQuadAngle()) {
      // Add additional mitreing points
      //scalar angleSign = 1.0;
      vector convexEdgesPlaneNormal =
        0.5*(convexEdgePlaneCNormal + convexEdgePlaneDNormal);
      plane planeM{featPt, convexEdgesPlaneNormal};
      const mousse::point internalPtF =
        concaveEdgeExternalPt
        + 2.0*(concaveEdgeLocalFeatPt - concaveEdgeExternalPt);
      pts.append
      (
        Vb
        {
          internalPtF,
          foamyHexMesh_.vertexCount() + pts.size(),
          Vb::vtInternalFeaturePoint,
          Pstream::myProcNo()
        }
      );
      const label internalPtFIndex{pts.last().index()};
      ftPtPairs_.addPointPair
      (
        concaveEdgeExternalPtIndex,
        pts.last().index()
      );
      const mousse::point externalPtG =
        internalPtF
        + 2.0*planeM.distance(internalPtF)*convexEdgesPlaneNormal;
      pts.append
      (
        Vb
        {
          externalPtG,
          foamyHexMesh_.vertexCount() + pts.size(),
          Vb::vtExternalFeaturePoint,
          Pstream::myProcNo()
        }
      );
      ftPtPairs_.addPointPair
      (
        internalPtFIndex,
        pts.last().index()
      );
    }
    if (debug) {
      for (label ptI = initialNumOfPoints; ptI < pts.size(); ++ptI) {
        Info << "Point " << ptI << " : ";
        meshTools::writeOBJ(Info, topoint(pts[ptI].point()));
      }
    }
    return true;
  } else if (pFEdgesTypes[extendedFeatureEdgeMesh::EXTERNAL] == 1
             && pFEdgesTypes[extendedFeatureEdgeMesh::INTERNAL] == 2
             && pEds.size() == 3) {
    if (debug) {
      Info << "nExternal == 1 && nInternal == 2" << endl;
    }
    const mousse::point& featPt = feMesh.points()[ptI];
    if (Pstream::parRun()
        && !foamyHexMesh_.decomposition().positionOnThisProcessor(featPt)) {
      return false;
    }
    label nVert = foamyHexMesh_.number_of_vertices();
    const label initialNumOfPoints = pts.size();
    const scalar ppDist = foamyHexMesh_.pointPairDistance(featPt);
    const vectorField& normals = feMesh.normals();
    const labelListList& edgeNormals = feMesh.edgeNormals();
    label convexEdgeI = -1;
    labelList concaveEdgesI{2, label(-1)};
    label nConcave = 0;
    FOR_ALL(pEds, i) {
      const extendedFeatureEdgeMesh::edgeStatus& eS = allEdStat[i];
      if (eS == extendedFeatureEdgeMesh::EXTERNAL) {
        convexEdgeI = pEds[i];
      } else if (eS == extendedFeatureEdgeMesh::INTERNAL) {
        concaveEdgesI[nConcave++] = pEds[i];
      } else if (eS == extendedFeatureEdgeMesh::FLAT) {
        WARNING_IN("mousse::conformalVoronoiMesh::"
          "createSpecialisedFeaturePoint")
          << "Edge " << eS << " is flat"
          << endl;
      } else {
        FATAL_ERROR_IN("mousse::conformalVoronoiMesh::"
                       "createSpecialisedFeaturePoint")
          << "Edge " << eS << " not concave/convex"
          << exit(FatalError);
      }
    }
    const vector& convexEdgePlaneANormal =
      normals[edgeNormals[convexEdgeI][0]];
    const vector& convexEdgePlaneBNormal =
      normals[edgeNormals[convexEdgeI][1]];
    // Intersect planes parallel to the concave edge planes offset
    // by ppDist and the plane defined by featPt and the edge vector.
    plane planeA
    {
      featPt - ppDist*convexEdgePlaneANormal,
      convexEdgePlaneANormal
    };
    plane planeB
    {
      featPt - ppDist*convexEdgePlaneBNormal,
      convexEdgePlaneBNormal
    };
    const vector& convexEdgeDir =
      feMesh.edgeDirection
      (
        convexEdgeI,
        ptI
      );
    // Todo,needed later but want to get rid of this.
    const mousse::point convexEdgeLocalFeatPt =
      featPt + ppDist*convexEdgeDir;
    // Finding the nearest point on the intersecting line to the edge
    // point. Floating point errors often occur using planePlaneIntersect
    plane planeF{convexEdgeLocalFeatPt, convexEdgeDir};
    const mousse::point convexEdgeExternalPt =
      planeF.planePlaneIntersect
      (
        planeA,
        planeB
      );
    // Redefine planes to be on the feature surfaces to project through
    planeA = plane{featPt, convexEdgePlaneANormal};
    planeB = plane{featPt, convexEdgePlaneBNormal};
    const mousse::point internalPtA =
      convexEdgeExternalPt
      + 2.0*planeA.distance(convexEdgeExternalPt)*convexEdgePlaneANormal;
    pts.append
    (
      Vb
      {
        internalPtA,
        foamyHexMesh_.vertexCount() + pts.size(),
        Vb::vtExternalFeaturePoint,
        Pstream::myProcNo()
      }
    );
    const label internalPtAIndex{pts.last().index()};
    const mousse::point internalPtB =
      convexEdgeExternalPt
      + 2.0*planeB.distance(convexEdgeExternalPt)*convexEdgePlaneBNormal;
    pts.append
    (
      Vb
      {
        internalPtB,
        foamyHexMesh_.vertexCount() + pts.size(),
        Vb::vtExternalFeaturePoint,
        Pstream::myProcNo()
      }
    );
    const label internalPtBIndex{pts.last().index()};
    // Add the internal points
    mousse::point externalPtD;
    mousse::point externalPtE;
    vector concaveEdgePlaneCNormal{vector::zero};
    vector concaveEdgePlaneDNormal{vector::zero};
    const labelList& convexEdgeNormals = edgeNormals[convexEdgeI];
    const labelList& concaveEdgeANormals = edgeNormals[concaveEdgesI[0]];
    const labelList& concaveEdgeBNormals = edgeNormals[concaveEdgesI[1]];
    FOR_ALL(convexEdgeNormals, edgeNormalI) {
      bool concaveEdgeA = false;
      bool concaveEdgeB = false;
      FOR_ALL(concaveEdgeANormals, edgeAnormalI) {
        const vector& convexNormal =
          normals[convexEdgeNormals[edgeNormalI]];
        const vector& concaveNormal =
          normals[concaveEdgeANormals[edgeAnormalI]];
        if (debug) {
          Info << "Angle between vectors = "
            << degAngleBetween(convexNormal, concaveNormal) << endl;
        }
        // Need a looser tolerance, because sometimes adjacent triangles
        // on the same surface will be slightly out of alignment.
        if (areParallel(convexNormal, concaveNormal, tolParallel)) {
          concaveEdgeA = true;
        }
      }
      FOR_ALL(concaveEdgeBNormals, edgeBnormalI) {
        const vector& convexNormal =
          normals[convexEdgeNormals[edgeNormalI]];
        const vector& concaveNormal =
          normals[concaveEdgeBNormals[edgeBnormalI]];
        if (debug) {
          Info << "Angle between vectors = "
            << degAngleBetween(convexNormal, concaveNormal) << endl;
        }
        // Need a looser tolerance, because sometimes adjacent triangles
        // on the same surface will be slightly out of alignment.
        if (areParallel(convexNormal, concaveNormal, tolParallel)) {
          concaveEdgeB = true;
        }
      }
      if ((concaveEdgeA && concaveEdgeB) || (!concaveEdgeA && !concaveEdgeB)) {
        WARNING_IN
        (
          "mousse::conformalVoronoiMesh"
          "::createSpecialisedFeaturePoint"
        )
        << "Both or neither of the concave edges share the convex "
        << "edge's normal."
        << " concaveEdgeA = " << concaveEdgeA
        << " concaveEdgeB = " << concaveEdgeB
        << endl;
        // Remove points that have just been added before returning
        for (label i = 0; i < 2; ++i) {
          pts.remove();
          nVert--;
        }
        return false;
      }
      if (concaveEdgeA) {
        FOR_ALL(concaveEdgeANormals, edgeAnormalI) {
          const vector& convexNormal =
            normals[convexEdgeNormals[edgeNormalI]];
          const vector& concaveNormal =
            normals[concaveEdgeANormals[edgeAnormalI]];
          if (!areParallel(convexNormal, concaveNormal, tolParallel)) {
            concaveEdgePlaneCNormal = concaveNormal;
            plane planeC{featPt, concaveEdgePlaneCNormal};
            externalPtD =
              internalPtA
              - 2.0*planeC.distance(internalPtA)*concaveEdgePlaneCNormal;
            pts.append
            (
              Vb
              {
                externalPtD,
                foamyHexMesh_.vertexCount() + pts.size(),
                Vb::vtInternalFeaturePoint,
                Pstream::myProcNo()
              }
            );
            ftPtPairs_.addPointPair
            (
              internalPtAIndex,
              pts.last().index()
            );
          }
        }
      }
      if (concaveEdgeB) {
        FOR_ALL(concaveEdgeBNormals, edgeBnormalI) {
          const vector& convexNormal =
            normals[convexEdgeNormals[edgeNormalI]];
          const vector& concaveNormal =
            normals[concaveEdgeBNormals[edgeBnormalI]];
          if (!areParallel(convexNormal, concaveNormal, tolParallel)) {
            concaveEdgePlaneDNormal = concaveNormal;
            plane planeD{featPt, concaveEdgePlaneDNormal};
            externalPtE =
              internalPtB
              - 2.0*planeD.distance(internalPtB)*concaveEdgePlaneDNormal;
            pts.append
            (
              Vb
              {
                externalPtE,
                foamyHexMesh_.vertexCount() + pts.size(),
                Vb::vtInternalFeaturePoint,
                Pstream::myProcNo()
              }
            );
            ftPtPairs_.addPointPair
            (
              internalPtBIndex,
              pts.last().index()
            );
          }
        }
      }
    }
    pts.append
    (
      Vb
      {
        convexEdgeExternalPt,
        foamyHexMesh_.vertexCount() + pts.size(),
        Vb::vtInternalFeaturePoint,
        Pstream::myProcNo()
      }
    );
    ftPtPairs_.addPointPair
    (
      internalPtBIndex,
      pts.last().index()
    );
    ftPtPairs_.addPointPair
    (
      internalPtAIndex,
      pts.last().index()
    );
    const scalar totalAngle =
      radToDeg
      (
        constant::mathematical::pi
        + radAngleBetween(convexEdgePlaneANormal, convexEdgePlaneBNormal)
      );
    if (totalAngle > foamyHexMeshControls_.maxQuadAngle()) {
      // Add additional mitreing points
      //scalar angleSign = 1.0;
      vector convexEdgesPlaneNormal =
        0.5*(concaveEdgePlaneCNormal + concaveEdgePlaneDNormal);
      plane planeM{featPt, convexEdgesPlaneNormal};
      const mousse::point internalPtF =
        convexEdgeExternalPt
        + 2.0*(convexEdgeLocalFeatPt - convexEdgeExternalPt);
      pts.append
      (
        Vb
        {
          internalPtF,
          foamyHexMesh_.vertexCount() + pts.size(),
          Vb::vtExternalFeaturePoint,
          Pstream::myProcNo()
        }
      );
      ftPtPairs_.addPointPair
      (
        pts[pts.size() - 2].index(),
        pts.last().index()
      );
      const mousse::point externalPtG =
        internalPtF
        - 2.0*planeM.distance(internalPtF)*convexEdgesPlaneNormal;
      pts.append
      (
        Vb
        {
          externalPtG,
          foamyHexMesh_.vertexCount() + pts.size(),
          Vb::vtInternalFeaturePoint,
          Pstream::myProcNo()
        }
      );
      ftPtPairs_.addPointPair
      (
        pts[pts.size() - 2].index(),
        pts.last().index()
      );
    }
    if (debug) {
      for (label ptI = initialNumOfPoints; ptI < pts.size(); ++ptI) {
        Info << "Point " << ptI << " "
          << indexedVertexEnum::vertexTypeNames_[pts[ptI].type()]
          << " : ";
        meshTools::writeOBJ(Info, topoint(pts[ptI].point()));
      }
    }
    return true;
  }
  return false;
}

