// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "sync_tools.hpp"
#include "patch_face_orientation.hpp"
#include "patch_edge_face_wave.hpp"
#include "oriented_surface.hpp"
#include "global_index.hpp"

using namespace mousse;

int main(int argc, char *argv[])
{
  #include "add_region_option.inc"
  argList::validArgs.append("faceZone");
  argList::validArgs.append("outsidePoint");
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_named_poly_mesh.inc"
  const word zoneName  = args[1];
  const point outsidePoint = args.argRead<point>(2);
  Info << "Orienting faceZone " << zoneName
    << " such that " << outsidePoint << " is outside"
    << nl << endl;
  const faceZone& fZone = mesh.faceZones()[zoneName];
  if (fZone.checkParallelSync())
  {
    FATAL_ERROR_IN(args.executable())
      << "Face zone " << fZone.name()
      << " is not parallel synchronised."
      << " Any coupled face also needs its coupled version to be included"
      << " and with opposite flipMap."
      << exit(FatalError);
  }
  const labelList& faceLabels = fZone;
  const indirectPrimitivePatch patch
  {
    IndirectList<face>(mesh.faces(), faceLabels),
    mesh.points()
  };
  const PackedBoolList isMasterFace{syncTools::getMasterFaces(mesh)};
  // Data on all edges and faces
  List<patchFaceOrientation> allEdgeInfo{patch.nEdges()};
  List<patchFaceOrientation> allFaceInfo{patch.size()};
  // Make sure we don't walk through
  // - slaves of coupled faces
  // - non-manifold edges
  {
    const polyBoundaryMesh& bm = mesh.boundaryMesh();
    label nProtected = 0;
    FOR_ALL(faceLabels, faceI)
    {
      const label meshFaceI = faceLabels[faceI];
      const label patchI = bm.whichPatch(meshFaceI);
      if (patchI != -1 && bm[patchI].coupled() && !isMasterFace[meshFaceI])
      {
        // Slave side. Mark so doesn't get visited.
        allFaceInfo[faceI] = orientedSurface::NOFLIP;
        nProtected++;
      }
    }
    Info << "Protected from visiting "
      << returnReduce(nProtected, sumOp<label>())
      << " slaves of coupled faces" << nl << endl;
  }

  {
    // Number of (master)faces per edge
    labelList nMasterFaces{patch.nEdges(), 0};
    FOR_ALL(faceLabels, faceI)
    {
      const label meshFaceI = faceLabels[faceI];
      if (isMasterFace[meshFaceI])
      {
        const labelList& fEdges = patch.faceEdges()[faceI];
        FOR_ALL(fEdges, fEdgeI)
        {
          nMasterFaces[fEdges[fEdgeI]]++;
        }
      }
    }
    syncTools::syncEdgeList
    (
      mesh,
      patch.meshEdges(mesh.edges(), mesh.pointEdges()),
      nMasterFaces,
      plusEqOp<label>(),
      label(0)
    );
    label nProtected = 0;
    FOR_ALL(nMasterFaces, edgeI)
    {
      if (nMasterFaces[edgeI] > 2)
      {
        allEdgeInfo[edgeI] = orientedSurface::NOFLIP;
        nProtected++;
      }
    }
    Info << "Protected from visiting "
      << returnReduce(nProtected, sumOp<label>())
      << " non-manifold edges" << nl << endl;
  }
  DynamicList<label> changedEdges;
  DynamicList<patchFaceOrientation> changedInfo;
  const scalar tol = PatchEdgeFaceWave
  <
    indirectPrimitivePatch,
    patchFaceOrientation
  >::propagationTol();
  int dummyTrackData;
  globalIndex globalFaces{patch.size()};
  while (true)
  {
    // Pick an unset face
    label unsetFaceI = labelMax;
    FOR_ALL(allFaceInfo, faceI)
    {
      if (allFaceInfo[faceI] == orientedSurface::UNVISITED)
      {
        unsetFaceI = globalFaces.toGlobal(faceI);
        break;
      }
    }
    reduce(unsetFaceI, minOp<label>());
    if (unsetFaceI == labelMax)
    {
      break;
    }
    label procI = globalFaces.whichProcID(unsetFaceI);
    label seedFaceI = globalFaces.toLocal(procI, unsetFaceI);
    Info << "Seeding from processor " << procI << " face " << seedFaceI
      << endl;
    if (procI == Pstream::myProcNo())
    {
      // Determine orientation of seedFace
      vector d = outsidePoint-patch.faceCentres()[seedFaceI];
      const vector& fn = patch.faceNormals()[seedFaceI];
      // Set information to correct orientation
      patchFaceOrientation& faceInfo = allFaceInfo[seedFaceI];
      faceInfo = orientedSurface::NOFLIP;
      if ((fn&d) < 0)
      {
        faceInfo.flip();
        Pout << "Face " << seedFaceI << " at "
          << patch.faceCentres()[seedFaceI]
          << " with normal " << fn
          << " needs to be flipped." << endl;
      }
      else
      {
        Pout << "Face " << seedFaceI << " at "
          << patch.faceCentres()[seedFaceI]
          << " with normal " << fn
          << " points in positive direction (cos = " << (fn&d)/mag(d)
          << ")" << endl;
      }
      const labelList& fEdges = patch.faceEdges()[seedFaceI];
      FOR_ALL(fEdges, fEdgeI)
      {
        label edgeI = fEdges[fEdgeI];
        patchFaceOrientation& edgeInfo = allEdgeInfo[edgeI];
        if
        (
          edgeInfo.updateEdge<int>
          (
            mesh,
            patch,
            edgeI,
            seedFaceI,
            faceInfo,
            tol,
            dummyTrackData
          )
        )
        {
          changedEdges.append(edgeI);
          changedInfo.append(edgeInfo);
        }
      }
    }
    if (returnReduce(changedEdges.size(), sumOp<label>()) == 0)
    {
      break;
    }
    // Walk
    PatchEdgeFaceWave
    <
      indirectPrimitivePatch,
      patchFaceOrientation
    > calc
    (
      mesh,
      patch,
      changedEdges,
      changedInfo,
      allEdgeInfo,
      allFaceInfo,
      returnReduce(patch.nEdges(), sumOp<label>())
    );
  }

  // Push master zone info over to slave (since slave faces never visited)
  {
    const polyBoundaryMesh& bm = mesh.boundaryMesh();
    labelList neiStatus
    (
      mesh.nFaces()-mesh.nInternalFaces(),
      orientedSurface::UNVISITED
    );
    FOR_ALL(faceLabels, i)
    {
      const label meshFaceI = faceLabels[i];
      if (!mesh.isInternalFace(meshFaceI))
      {
        neiStatus[meshFaceI-mesh.nInternalFaces()] =
          allFaceInfo[i].flipStatus();
      }
    }
    syncTools::swapBoundaryFaceList(mesh, neiStatus);
    FOR_ALL(faceLabels, i)
    {
      const label meshFaceI = faceLabels[i];
      const label patchI = bm.whichPatch(meshFaceI);
      if (patchI != -1 && bm[patchI].coupled() && !isMasterFace[meshFaceI])
      {
        // Slave side. Take flipped from neighbour
        label bFaceI = meshFaceI-mesh.nInternalFaces();
        if (neiStatus[bFaceI] == orientedSurface::NOFLIP)
        {
          allFaceInfo[i] = orientedSurface::FLIP;
        }
        else if (neiStatus[bFaceI] == orientedSurface::FLIP)
        {
          allFaceInfo[i] = orientedSurface::NOFLIP;
        }
        else
        {
          FATAL_ERROR_IN(args.executable())
            << "Incorrect status for face " << meshFaceI
            << abort(FatalError);
        }
      }
    }
  }
  // Convert to flipmap and adapt faceZones
  boolList newFlipMap{allFaceInfo.size(), false};
  label nChanged = 0;
  FOR_ALL(allFaceInfo, faceI)
  {
    if (allFaceInfo[faceI] == orientedSurface::NOFLIP)
    {
      newFlipMap[faceI] = false;
    }
    else if (allFaceInfo[faceI] == orientedSurface::FLIP)
    {
      newFlipMap[faceI] = true;
    }
    else
    {
      FATAL_ERROR_IN(args.executable())
        << "Problem : unvisited face " << faceI
        << " centre:" << mesh.faceCentres()[faceLabels[faceI]]
        << abort(FatalError);
    }
    if (fZone.flipMap()[faceI] != newFlipMap[faceI])
    {
      nChanged++;
    }
  }
  reduce(nChanged, sumOp<label>());
  if (nChanged > 0)
  {
    Info << "Flipping " << nChanged << " out of "
      << globalFaces.size() << " faces." << nl << endl;
    mesh.faceZones()[zoneName].resetAddressing(faceLabels, newFlipMap);
    if (!mesh.faceZones().write())
    {
      FATAL_ERROR_IN(args.executable())
        << "Failed writing faceZones" << exit(FatalError);
    }
  }
  Info << "\nEnd\n" << endl;
  return 0;
}
