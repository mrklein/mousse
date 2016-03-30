// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_tools.hpp"
#include "poly_mesh.hpp"
#include "indirect_primitive_patch.hpp"
#include "global_mesh_data.hpp"


// Member Functions 
template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
mousse::tmp<mousse::pointField>
mousse::PatchTools::pointNormals
(
  const polyMesh& mesh,
  const PrimitivePatch<Face, FaceList, PointField, PointType>& p
)
{
  const globalMeshData& globalData = mesh.globalData();
  const indirectPrimitivePatch& coupledPatch = globalData.coupledPatch();
  const Map<label>& coupledPatchMP = coupledPatch.meshPointMap();
  const mapDistribute& map = globalData.globalPointSlavesMap();
  const globalIndexAndTransform& transforms = globalData.globalTransforms();
  // Combine normals. Note: do on all master points. Cannot just use
  // patch points since the master point does not have to be on the
  // patch!
  pointField coupledPointNormals{map.constructSize(), vector::zero};

  {
    // Collect local pointFaces (sized on patch points only)
    List<List<point>> pointFaceNormals{map.constructSize()};
    FOR_ALL(p.meshPoints(), patchPointI) {
      label meshPointI = p.meshPoints()[patchPointI];
      Map<label>::const_iterator fnd = coupledPatchMP.find(meshPointI);
      if (fnd != coupledPatchMP.end()) {
        label coupledPointI = fnd();
        List<point>& pNormals = pointFaceNormals[coupledPointI];
        const labelList& pFaces = p.pointFaces()[patchPointI];
        pNormals.setSize(pFaces.size());
        FOR_ALL(pFaces, i) {
          pNormals[i] = p.faceNormals()[pFaces[i]];
        }
      }
    }
    // Pull remote data into local slots
    map.distribute
    (
      transforms,
      pointFaceNormals,
      mapDistribute::transform()
    );
    // Combine all face normals (-local, -remote,untransformed,
    //  -remote,transformed)
    const labelListList& slaves = globalData.globalPointSlaves();
    const labelListList& transformedSlaves =
      globalData.globalPointTransformedSlaves();
    FOR_ALL(slaves, coupledPointI) {
      const labelList& slaveSlots = slaves[coupledPointI];
      const labelList& transformedSlaveSlots =
        transformedSlaves[coupledPointI];
      point& n = coupledPointNormals[coupledPointI];
      // Local entries
      const List<point>& local = pointFaceNormals[coupledPointI];
      label nFaces =
        local.size() + slaveSlots.size() + transformedSlaveSlots.size();
      n = sum(local);
      // Add any remote face normals
      FOR_ALL(slaveSlots, i) {
        n += sum(pointFaceNormals[slaveSlots[i]]);
      }
      FOR_ALL(transformedSlaveSlots, i) {
        n += sum(pointFaceNormals[transformedSlaveSlots[i]]);
      }
      if (nFaces >= 1) {
        n /= mag(n) + VSMALL;
      }
      // Put back into slave slots
      FOR_ALL(slaveSlots, i) {
        coupledPointNormals[slaveSlots[i]] = n;
      }
      FOR_ALL(transformedSlaveSlots, i) {
        coupledPointNormals[transformedSlaveSlots[i]] = n;
      }
    }
    // Send back
    map.reverseDistribute
    (
      transforms,
      coupledPointNormals.size(),
      coupledPointNormals,
      mapDistribute::transform()
    );
  }
  // 1. Start off with local normals (note:without calculating pointNormals
  //    to avoid them being stored)
  tmp<pointField> textrudeN{new pointField{p.nPoints(), vector::zero}};
  pointField& extrudeN = textrudeN();

  {
    const faceList& localFaces = p.localFaces();
    const vectorField& faceNormals = p.faceNormals();
    FOR_ALL(localFaces, faceI) {
      const face& f = localFaces[faceI];
      const vector& n = faceNormals[faceI];
      FOR_ALL(f, fp) {
        extrudeN[f[fp]] += n;
      }
    }
    extrudeN /= mag(extrudeN)+VSMALL;
  }
  // 2. Override patch normals on coupled points
  FOR_ALL(p.meshPoints(), patchPointI) {
    label meshPointI = p.meshPoints()[patchPointI];
    Map<label>::const_iterator fnd = coupledPatchMP.find(meshPointI);
    if (fnd != coupledPatchMP.end()) {
      label coupledPointI = fnd();
      extrudeN[patchPointI] = coupledPointNormals[coupledPointI];
    }
  }
  return textrudeN;
}


template
<
  class Face,
  template<class> class FaceList,
  class PointField,
  class PointType
>
mousse::tmp<mousse::pointField>
mousse::PatchTools::edgeNormals
(
  const polyMesh& mesh,
  const PrimitivePatch<Face, FaceList, PointField, PointType>& p,
  const labelList& patchEdges,
  const labelList& coupledEdges
)
{
  // 1. Start off with local normals
  tmp<pointField> tedgeNormals{new pointField{p.nEdges(), vector::zero}};
  pointField& edgeNormals = tedgeNormals();

  {
    const labelListList& edgeFaces = p.edgeFaces();
    const vectorField& faceNormals = p.faceNormals();
    FOR_ALL(edgeFaces, edgeI) {
      const labelList& eFaces = edgeFaces[edgeI];
      FOR_ALL(eFaces, i) {
        edgeNormals[edgeI] += faceNormals[eFaces[i]];
      }
    }
    edgeNormals /= mag(edgeNormals)+VSMALL;
  }

  const globalMeshData& globalData = mesh.globalData();
  const mapDistribute& map = globalData.globalEdgeSlavesMap();

  // Convert patch-edge data into cpp-edge data
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //- Construct with all data in consistent orientation
  pointField cppEdgeData{map.constructSize(), vector::zero};
  FOR_ALL(patchEdges, i) {
    label patchEdgeI = patchEdges[i];
    label coupledEdgeI = coupledEdges[i];
    cppEdgeData[coupledEdgeI] = edgeNormals[patchEdgeI];
  }
  // Synchronise
  // ~~~~~~~~~~~
  globalData.syncData
  (
    cppEdgeData,
    globalData.globalEdgeSlaves(),
    globalData.globalEdgeTransformedSlaves(),
    map,
    globalData.globalTransforms(),
    plusEqOp<point>(),              // add since normalised later on
    mapDistribute::transform()
  );
  cppEdgeData /= mag(cppEdgeData)+VSMALL;
  // Back from cpp-edge to patch-edge data
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  FOR_ALL(patchEdges, i) {
    label patchEdgeI = patchEdges[i];
    label coupledEdgeI = coupledEdges[i];
    edgeNormals[patchEdgeI] = cppEdgeData[coupledEdgeI];
  }
  return tedgeNormals;
}
