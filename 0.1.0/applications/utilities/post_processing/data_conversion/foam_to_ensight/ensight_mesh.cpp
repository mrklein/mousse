// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_mesh.hpp"
#include "arg_list.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"
#include "global_mesh_data.hpp"
#include "pstream_combine_reduce_ops.hpp"
#include "processor_poly_patch.hpp"
#include "cell_modeller.hpp"
#include "iomanip.hpp"
#include "itoa.hpp"
#include "global_index.hpp"
#include "map_distribute.hpp"
#include "string_list_ops.hpp"
#include "ensight_binary_stream.hpp"
#include "ensight_ascii_stream.hpp"
#include <fstream>


// Private Functions
void mousse::ensightMesh::correct()
{
  patchPartOffset_ = 2;
  meshCellSets_.setSize(mesh_.nCells());
  boundaryFaceSets_.setSize(mesh_.boundary().size());
  allPatchNames_.clear();
  patchNames_.clear();
  nPatchPrims_ = 0;
  faceZoneFaceSets_.setSize(mesh_.faceZones().size());
  faceZoneNames_.clear();
  nFaceZonePrims_ = 0;
  boundaryFaceToBeIncluded_.clear();
  if (!noPatches_) {
    // Patches are output. Check that they're synced.
    mesh_.boundaryMesh().checkParallelSync(true);
    allPatchNames_ = mesh_.boundaryMesh().names();
    if (Pstream::parRun()) {
      allPatchNames_.setSize
      (
        mesh_.boundary().size() - mesh_.globalData().processorPatches().size()
      );
    }
    if (patches_) {
      if (patchPatterns_.empty()) {
        FOR_ALL(allPatchNames_, nameI) {
          patchNames_.insert(allPatchNames_[nameI]);
        }
      } else {
        // Find patch names which match that requested at command-line
        FOR_ALL(allPatchNames_, nameI) {
          const word& patchName = allPatchNames_[nameI];
          if (findStrings(patchPatterns_, patchName)) {
            patchNames_.insert(patchName);
          }
        }
      }
    }
  }
  if (patchNames_.size()) {
    // no internalMesh
    patchPartOffset_ = 1;
  } else {
    const cellShapeList& cellShapes = mesh_.cellShapes();
    const cellModel& tet = *(cellModeller::lookup("tet"));
    const cellModel& pyr = *(cellModeller::lookup("pyr"));
    const cellModel& prism = *(cellModeller::lookup("prism"));
    const cellModel& wedge = *(cellModeller::lookup("wedge"));
    const cellModel& hex = *(cellModeller::lookup("hex"));
    // Count the shapes
    labelList& tets = meshCellSets_.tets;
    labelList& pyrs = meshCellSets_.pyrs;
    labelList& prisms = meshCellSets_.prisms;
    labelList& wedges = meshCellSets_.wedges;
    labelList& hexes = meshCellSets_.hexes;
    labelList& polys = meshCellSets_.polys;
    label nTets = 0;
    label nPyrs = 0;
    label nPrisms = 0;
    label nWedges = 0;
    label nHexes = 0;
    label nPolys = 0;
    FOR_ALL(cellShapes, cellI) {
      const cellShape& cellShape = cellShapes[cellI];
      const cellModel& cellModel = cellShape.model();
      if (cellModel == tet) {
        tets[nTets++] = cellI;
      } else if (cellModel == pyr) {
        pyrs[nPyrs++] = cellI;
      } else if (cellModel == prism) {
        prisms[nPrisms++] = cellI;
      } else if (cellModel == wedge) {
        wedges[nWedges++] = cellI;
      } else if (cellModel == hex) {
        hexes[nHexes++] = cellI;
      } else {
        polys[nPolys++] = cellI;
      }
    }
    tets.setSize(nTets);
    pyrs.setSize(nPyrs);
    prisms.setSize(nPrisms);
    wedges.setSize(nWedges);
    hexes.setSize(nHexes);
    polys.setSize(nPolys);
    meshCellSets_.nTets = nTets;
    reduce(meshCellSets_.nTets, sumOp<label>());
    meshCellSets_.nPyrs = nPyrs;
    reduce(meshCellSets_.nPyrs, sumOp<label>());
    meshCellSets_.nPrisms = nPrisms;
    reduce(meshCellSets_.nPrisms, sumOp<label>());
    meshCellSets_.nHexesWedges = nWedges+nHexes;
    reduce(meshCellSets_.nHexesWedges, sumOp<label>());
    meshCellSets_.nPolys = nPolys;
    reduce(meshCellSets_.nPolys, sumOp<label>());
    // Determine parallel shared points
    globalPointsPtr_ =
      mesh_.globalData().mergePoints(pointToGlobal_, uniquePointMap_);
  }
  if (!noPatches_) {
    FOR_ALL(mesh_.boundary(), patchi) {
      if (!mesh_.boundary()[patchi].size())
        continue;
      const polyPatch& p = mesh_.boundaryMesh()[patchi];
      labelList& tris = boundaryFaceSets_[patchi].tris;
      labelList& quads = boundaryFaceSets_[patchi].quads;
      labelList& polys = boundaryFaceSets_[patchi].polys;
      tris.setSize(p.size());
      quads.setSize(p.size());
      polys.setSize(p.size());
      label nTris = 0;
      label nQuads = 0;
      label nPolys = 0;
      FOR_ALL(p, faceI) {
        const face& f = p[faceI];
        if (f.size() == 3) {
          tris[nTris++] = faceI;
        } else if (f.size() == 4) {
          quads[nQuads++] = faceI;
        } else {
          polys[nPolys++] = faceI;
        }
      }
      tris.setSize(nTris);
      quads.setSize(nQuads);
      polys.setSize(nPolys);
    }
  }
  FOR_ALL(allPatchNames_, patchi) {
    const word& patchName = allPatchNames_[patchi];
    nFacePrimitives nfp;
    if (patchNames_.empty() || patchNames_.found(patchName)) {
      if (mesh_.boundary()[patchi].size()) {
        nfp.nTris   = boundaryFaceSets_[patchi].tris.size();
        nfp.nQuads  = boundaryFaceSets_[patchi].quads.size();
        nfp.nPolys  = boundaryFaceSets_[patchi].polys.size();
      }
    }
    reduce(nfp.nTris, sumOp<label>());
    reduce(nfp.nQuads, sumOp<label>());
    reduce(nfp.nPolys, sumOp<label>());
    nPatchPrims_.insert(patchName, nfp);
  }
  // faceZones
  if (faceZones_) {
    wordList faceZoneNamesAll = mesh_.faceZones().names();
    // Need to sort the list of all face zones since the index may vary
    // from processor to processor...
    sort(faceZoneNamesAll);
    // Find faceZone names which match that requested at command-line
    FOR_ALL(faceZoneNamesAll, nameI) {
      const word& zoneName = faceZoneNamesAll[nameI];
      if (findStrings(faceZonePatterns_, zoneName)) {
        faceZoneNames_.insert(zoneName);
      }
    }
    // Build list of boundary faces to be exported
    boundaryFaceToBeIncluded_.setSize
    (
      mesh_.nFaces() - mesh_.nInternalFaces(),
      1
    );
    FOR_ALL(mesh_.boundaryMesh(), patchI) {
      const polyPatch& pp = mesh_.boundaryMesh()[patchI];
      if (isA<processorPolyPatch>(pp)
          && !refCast<const processorPolyPatch>(pp).owner()) {
        label bFaceI = pp.start() - mesh_.nInternalFaces();
        FOR_ALL(pp, i) {
          boundaryFaceToBeIncluded_[bFaceI++] = 0;
        }
      }
    }
    // Count face types in each faceZone
    FOR_ALL(faceZoneNamesAll, zoneI) {
      const word& zoneName = faceZoneNamesAll[zoneI];
      const label faceZoneId = mesh_.faceZones().findZoneID(zoneName);
      const faceZone& fz = mesh_.faceZones()[faceZoneId];
      if (fz.size()) {
        labelList& tris = faceZoneFaceSets_[faceZoneId].tris;
        labelList& quads = faceZoneFaceSets_[faceZoneId].quads;
        labelList& polys = faceZoneFaceSets_[faceZoneId].polys;
        tris.setSize(fz.size());
        quads.setSize(fz.size());
        polys.setSize(fz.size());
        label nTris = 0;
        label nQuads = 0;
        label nPolys = 0;
        label faceCounter = 0;
        FOR_ALL(fz, i) {
          label faceI = fz[i];
          // Avoid counting faces on processor boundaries twice
          if (faceToBeIncluded(faceI)) {
            const face& f = mesh_.faces()[faceI];
            if (f.size() == 3) {
              tris[nTris++] = faceCounter;
            } else if (f.size() == 4) {
              quads[nQuads++] = faceCounter;
            } else {
              polys[nPolys++] = faceCounter;
            }
            ++faceCounter;
          }
        }
        tris.setSize(nTris);
        quads.setSize(nQuads);
        polys.setSize(nPolys);
      }
    }
    FOR_ALL(faceZoneNamesAll, zoneI) {
      const word& zoneName = faceZoneNamesAll[zoneI];
      nFacePrimitives nfp;
      const label faceZoneId = mesh_.faceZones().findZoneID(zoneName);
      if (faceZoneNames_.found(zoneName)) {
        if (faceZoneFaceSets_[faceZoneId].tris.size()
            || faceZoneFaceSets_[faceZoneId].quads.size()
            || faceZoneFaceSets_[faceZoneId].polys.size()) {
          nfp.nTris   = faceZoneFaceSets_[faceZoneId].tris.size();
          nfp.nQuads  = faceZoneFaceSets_[faceZoneId].quads.size();
          nfp.nPolys  = faceZoneFaceSets_[faceZoneId].polys.size();
        }
      }
      reduce(nfp.nTris, sumOp<label>());
      reduce(nfp.nQuads, sumOp<label>());
      reduce(nfp.nPolys, sumOp<label>());
      nFaceZonePrims_.insert(zoneName, nfp);
    }
  }
}


// Constructors 
mousse::ensightMesh::ensightMesh
(
  const fvMesh& mesh,
  const bool noPatches,
  const bool patches,
  const wordReList& patchPatterns,
  const bool faceZones,
  const wordReList& faceZonePatterns,
  const bool binary
)
:
  mesh_{mesh},
  noPatches_{noPatches},
  patches_{patches},
  patchPatterns_{patchPatterns},
  faceZones_{faceZones},
  faceZonePatterns_{faceZonePatterns},
  binary_{binary},
  meshCellSets_{mesh.nCells()}
{
  correct();
}


// Destructor 
mousse::ensightMesh::~ensightMesh()
{}


// Member Functions 
bool mousse::ensightMesh::faceToBeIncluded(const label faceI) const
{
  bool res = false;
  if (mesh_.isInternalFace(faceI)) {
    res = true;
  } else {
    res = boundaryFaceToBeIncluded_[faceI-mesh_.nInternalFaces()];
  }
  return res;
}


void mousse::ensightMesh::barrier()
{
  label appI = 0;
  reduce(appI,maxOp<label>());
}


mousse::cellShapeList mousse::ensightMesh::map
(
  const cellShapeList& cellShapes,
  const labelList& prims,
  const labelList& pointToGlobal
) const
{
  cellShapeList mcsl(prims.size());
  FOR_ALL(prims, i) {
    mcsl[i] = cellShapes[prims[i]];
    inplaceRenumber(pointToGlobal, mcsl[i]);
  }
  return mcsl;
}


mousse::cellShapeList mousse::ensightMesh::map
(
  const cellShapeList& cellShapes,
  const labelList& hexes,
  const labelList& wedges,
  const labelList& pointToGlobal
) const
{
  cellShapeList mcsl{hexes.size() + wedges.size()};
  FOR_ALL(hexes, i) {
    mcsl[i] = cellShapes[hexes[i]];
    inplaceRenumber(pointToGlobal, mcsl[i]);
  }
  label offset = hexes.size();
  const cellModel& hex = *(cellModeller::lookup("hex"));
  labelList hexLabels{8};
  FOR_ALL(wedges, i) {
    const cellShape& cellPoints = cellShapes[wedges[i]];
    hexLabels[0] = cellPoints[0];
    hexLabels[1] = cellPoints[1];
    hexLabels[2] = cellPoints[0];
    hexLabels[3] = cellPoints[2];
    hexLabels[4] = cellPoints[3];
    hexLabels[5] = cellPoints[4];
    hexLabels[6] = cellPoints[6];
    hexLabels[7] = cellPoints[5];
    mcsl[i + offset] = cellShape(hex, hexLabels);
    inplaceRenumber(pointToGlobal, mcsl[i + offset]);
  }
  return mcsl;
}


void mousse::ensightMesh::writePrims
(
  const cellShapeList& cellShapes,
  ensightStream& ensightGeometryFile
) const
{
  if (!cellShapes.size())
    return;
  // Create a temp int array
  if (ensightGeometryFile.ascii()) {
    // Workaround for paraview issue : write one cell per line
    FOR_ALL(cellShapes, i) {
      const cellShape& cellPoints = cellShapes[i];
      List<int> temp{cellPoints.size()};
      FOR_ALL(cellPoints, pointI) {
        temp[pointI] = cellPoints[pointI] + 1;
      }
      ensightGeometryFile.write(temp);
    }
  } else {
    // All the cellShapes have the same number of elements!
    int numIntElem = cellShapes.size()*cellShapes[0].size();
    List<int> temp{numIntElem};
    int n = 0;
    FOR_ALL(cellShapes, i) {
      const cellShape& cellPoints = cellShapes[i];
      FOR_ALL(cellPoints, pointI) {
        temp[n] = cellPoints[pointI] + 1;
        n++;
      }
    }
    ensightGeometryFile.write(temp);
  }
}


void mousse::ensightMesh::writePolysNFaces
(
  const labelList& polys,
  const cellList& cellFaces,
  ensightStream& ensightGeometryFile
) const
{
  FOR_ALL(polys, i) {
    ensightGeometryFile.write(cellFaces[polys[i]].size());
  }
}


void mousse::ensightMesh::writePolysNPointsPerFace
(
  const labelList& polys,
  const cellList& cellFaces,
  const faceList& faces,
  ensightStream& ensightGeometryFile
) const
{
  FOR_ALL(polys, i) {
    const labelList& cf = cellFaces[polys[i]];
    FOR_ALL(cf, faceI) {
      ensightGeometryFile.write(faces[cf[faceI]].size());
    }
  }
}


void mousse::ensightMesh::writePolysPoints
(
  const labelList& polys,
  const cellList& cellFaces,
  const faceList& faces,
  const labelList& faceOwner,
  ensightStream& ensightGeometryFile
) const
{
  FOR_ALL(polys, i) {
    const labelList& cf = cellFaces[polys[i]];
    FOR_ALL(cf, faceI) {
      const label faceId = cf[faceI];
      const face& f = faces[faceId];  // points of face (in global points)
      const label np = f.size();
      bool reverseOrder = false;
      if (faceId >= faceOwner.size()) {
        // Boundary face.
        // Nothing should be done for processor boundary.
        // The current cell always owns them. Given that we
        // are reverting the
        // order when the cell is the neighbour to the face,
        // the orientation of
        // all the boundaries, no matter if they are "real"
        // or processorBoundaries, is consistent.
      } else {
        if (faceOwner[faceId] != polys[i]) {
          reverseOrder = true;
        }
      }
      // If the face owner is the current cell, write the points
      // in the standard order.
      // If the face owner is not the current cell, write the points
      // in reverse order.
      // EnSight prefers to have all the faces of an nfaced cell
      // oriented in the same way.
      List<int> temp{np};
      FOR_ALL(f, pointI) {
        if (reverseOrder) {
          temp[np-1-pointI] = f[pointI] + 1;
        } else {
          temp[pointI] = f[pointI] + 1;
        }
      }
      ensightGeometryFile.write(temp);
    }
  }
}


void mousse::ensightMesh::writeAllPolys
(
  const labelList& pointToGlobal,
  ensightStream& ensightGeometryFile
) const
{
  if (meshCellSets_.nPolys) {
    const cellList& cellFaces = mesh_.cells();
    const labelList& faceOwner = mesh_.faceOwner();
    // Renumber faces to use global point numbers
    faceList faces{mesh_.faces()};
    FOR_ALL(faces, i) {
      inplaceRenumber(pointToGlobal, faces[i]);
    }
    if (Pstream::master()) {
      ensightGeometryFile.write("nfaced");
      ensightGeometryFile.write(meshCellSets_.nPolys);
    }
    // Number of faces for each poly cell
    if (Pstream::master()) {
      // Master
      writePolysNFaces
      (
        meshCellSets_.polys,
        cellFaces,
        ensightGeometryFile
      );
      // Slaves
      for (int slave=1; slave<Pstream::nProcs(); slave++) {
        IPstream fromSlave{Pstream::scheduled, slave};
        labelList polys{fromSlave};
        cellList cellFaces{fromSlave};
        writePolysNFaces
        (
          polys,
          cellFaces,
          ensightGeometryFile
        );
      }
    } else {
      OPstream toMaster{Pstream::scheduled, Pstream::masterNo()};
      toMaster << meshCellSets_.polys << cellFaces;
    }
    // Number of points for each face of the above list
    if (Pstream::master()) {
      // Master
      writePolysNPointsPerFace
      (
        meshCellSets_.polys,
        cellFaces,
        faces,
        ensightGeometryFile
      );
      // Slaves
      for (int slave=1; slave<Pstream::nProcs(); slave++) {
        IPstream fromSlave{Pstream::scheduled, slave};
        labelList polys{fromSlave};
        cellList cellFaces{fromSlave};
        faceList faces{fromSlave};
        writePolysNPointsPerFace
        (
          polys,
          cellFaces,
          faces,
          ensightGeometryFile
        );
      }
    } else {
      OPstream toMaster{Pstream::scheduled, Pstream::masterNo()};
      toMaster << meshCellSets_.polys << cellFaces << faces;
    }
    // List of points id for each face of the above list
    if (Pstream::master()) {
      // Master
      writePolysPoints
      (
        meshCellSets_.polys,
        cellFaces,
        faces,
        faceOwner,
        ensightGeometryFile
      );
      // Slaves
      for (int slave=1; slave<Pstream::nProcs(); slave++) {
        IPstream fromSlave{Pstream::scheduled, slave};
        labelList polys{fromSlave};
        cellList cellFaces{fromSlave};
        faceList faces{fromSlave};
        labelList faceOwner{fromSlave};
        writePolysPoints
        (
          polys,
          cellFaces,
          faces,
          faceOwner,
          ensightGeometryFile
        );
      }
    } else {
      OPstream toMaster{Pstream::scheduled, Pstream::masterNo()};
      toMaster << meshCellSets_.polys << cellFaces << faces << faceOwner;
    }
  }
}


void mousse::ensightMesh::writeAllPrims
(
  const char* key,
  const label nPrims,
  const cellShapeList& cellShapes,
  ensightStream& ensightGeometryFile
) const
{
  if (!nPrims)
    return;
  if (Pstream::master()) {
    ensightGeometryFile.write(key);
    ensightGeometryFile.write(nPrims);
    writePrims(cellShapes, ensightGeometryFile);
    for (int slave=1; slave<Pstream::nProcs(); slave++) {
      IPstream fromSlave{Pstream::scheduled, slave};
      cellShapeList cellShapes{fromSlave};
      writePrims(cellShapes, ensightGeometryFile);
    }
  } else {
    OPstream toMaster{Pstream::scheduled, Pstream::masterNo()};
    toMaster << cellShapes;
  }
}


void mousse::ensightMesh::writeFacePrims
(
  const faceList& patchFaces,
  ensightStream& ensightGeometryFile
) const
{
  FOR_ALL(patchFaces, i) {
    const face& patchFace = patchFaces[i];
    List<int> temp{patchFace.size()};
    FOR_ALL(patchFace, pointI) {
      temp[pointI] = patchFace[pointI] + 1;
    }
    ensightGeometryFile.write(temp);
  }
}


void mousse::ensightMesh::writeAllFacePrims
(
  const char* key,
  const labelList& prims,
  const label nPrims,
  const faceList& patchFaces,
  ensightStream& ensightGeometryFile
) const
{
  if (!nPrims)
    return;
  if (Pstream::master()) {
    ensightGeometryFile.write(key);
    ensightGeometryFile.write(nPrims);
    writeFacePrims
    (
      UIndirectList<face>(patchFaces, prims)(),
      ensightGeometryFile
    );
    for (int slave=1; slave<Pstream::nProcs(); slave++) {
      IPstream fromSlave{Pstream::scheduled, slave};
      faceList patchFaces{fromSlave};
      writeFacePrims(patchFaces, ensightGeometryFile);
    }
  } else {
    OPstream toMaster{Pstream::scheduled, Pstream::masterNo()};
    toMaster << UIndirectList<face>(patchFaces, prims);
  }
}


void mousse::ensightMesh::writeNSidedNPointsPerFace
(
  const faceList& patchFaces,
  ensightStream& ensightGeometryFile
) const
{
  FOR_ALL(patchFaces, i) {
    ensightGeometryFile.write(patchFaces[i].size());
  }
}


void mousse::ensightMesh::writeNSidedPoints
(
  const faceList& patchFaces,
  ensightStream& ensightGeometryFile
) const
{
  writeFacePrims(patchFaces, ensightGeometryFile);
}


void mousse::ensightMesh::writeAllNSided
(
  const labelList& prims,
  const label nPrims,
  const faceList& patchFaces,
  ensightStream& ensightGeometryFile
) const
{
  if (!nPrims)
    return;
  if (Pstream::master()) {
    ensightGeometryFile.write("nsided");
    ensightGeometryFile.write(nPrims);
  }
  // Number of points for each face
  if (Pstream::master()) {
    writeNSidedNPointsPerFace
    (
      UIndirectList<face>(patchFaces, prims)(),
      ensightGeometryFile
    );
    for (int slave=1; slave<Pstream::nProcs(); slave++) {
      IPstream fromSlave{Pstream::scheduled, slave};
      faceList patchFaces{fromSlave};
      writeNSidedNPointsPerFace
      (
        patchFaces,
        ensightGeometryFile
      );
    }
  } else {
    OPstream toMaster{Pstream::scheduled, Pstream::masterNo()};
    toMaster << UIndirectList<face>(patchFaces, prims);
  }
  // List of points id for each face
  if (Pstream::master()) {
    writeNSidedPoints
    (
      UIndirectList<face>(patchFaces, prims)(),
      ensightGeometryFile
    );
    for (int slave=1; slave<Pstream::nProcs(); slave++) {
      IPstream fromSlave{Pstream::scheduled, slave};
      faceList patchFaces{fromSlave};
      writeNSidedPoints(patchFaces, ensightGeometryFile);
    }
  } else {
    OPstream toMaster{Pstream::scheduled, Pstream::masterNo()};
    toMaster << UIndirectList<face>(patchFaces, prims);
  }
}


void mousse::ensightMesh::writeAllPoints
(
  const label ensightPartI,
  const word& ensightPartName,
  const pointField& uniquePoints,
  const label nPoints,
  ensightStream& ensightGeometryFile
) const
{
  barrier();
  if (Pstream::master()) {
    ensightGeometryFile.writePartHeader(ensightPartI);
    ensightGeometryFile.write(ensightPartName.c_str());
    ensightGeometryFile.write("coordinates");
    ensightGeometryFile.write(nPoints);
    for (direction d=0; d<vector::nComponents; d++) {
      ensightGeometryFile.write(uniquePoints.component(d));
      for (int slave=1; slave<Pstream::nProcs(); slave++) {
        IPstream fromSlave{Pstream::scheduled, slave};
        scalarField patchPointsComponent{fromSlave};
        ensightGeometryFile.write(patchPointsComponent);
      }
    }
  } else {
    for (direction d=0; d<vector::nComponents; d++) {
      OPstream toMaster{Pstream::scheduled, Pstream::masterNo()};
      toMaster << uniquePoints.component(d);
    }
  }
}


void mousse::ensightMesh::write
(
  const fileName& postProcPath,
  const word& prepend,
  const label timeIndex,
  const bool meshMoving,
  Ostream& /*ensightCaseFile*/
) const
{
  const Time& runTime = mesh_.time();
  const cellShapeList& cellShapes = mesh_.cellShapes();
  word timeFile = prepend;
  if (timeIndex == 0) {
    timeFile += "0000.";
  } else if (meshMoving) {
    timeFile += itoa(timeIndex) + '.';
  }
  // set the filename of the ensight file
  fileName ensightGeometryFileName = timeFile + "mesh";
  ensightStream* ensightGeometryFilePtr = nullptr;
  if (Pstream::master()) {
    if (binary_) {
      ensightGeometryFilePtr =
        new ensightBinaryStream
        {
          postProcPath/ensightGeometryFileName,
          runTime
        };
      ensightGeometryFilePtr->write("C binary");
    } else {
      ensightGeometryFilePtr =
        new ensightAsciiStream
        {
          postProcPath/ensightGeometryFileName,
          runTime
        };
    }
  }
  ensightStream& ensightGeometryFile = *ensightGeometryFilePtr;
  if (Pstream::master()) {
    string desc = string("written by mousse-") + mousse::mousse_version;
    ensightGeometryFile.write("EnSight Geometry File");
    ensightGeometryFile.write(desc.c_str());
    ensightGeometryFile.write("node id assign");
    ensightGeometryFile.write("element id assign");
  }
  if (patchNames_.empty()) {
    label nPoints = globalPoints().size();
    const pointField uniquePoints(mesh_.points(), uniquePointMap_);
    writeAllPoints
    (
      1,
      "internalMesh",
      uniquePoints,
      nPoints,
      ensightGeometryFile
    );
    writeAllPrims
    (
      "hexa8",
      meshCellSets_.nHexesWedges,
      map         // Rewrite cellShapes to global numbering
      (
        cellShapes,
        meshCellSets_.hexes,
        meshCellSets_.wedges,
        pointToGlobal_
      ),
      ensightGeometryFile
    );
    writeAllPrims
    (
      "penta6",
      meshCellSets_.nPrisms,
      map(cellShapes, meshCellSets_.prisms, pointToGlobal_),
      ensightGeometryFile
    );
    writeAllPrims
    (
      "pyramid5",
      meshCellSets_.nPyrs,
      map(cellShapes, meshCellSets_.pyrs, pointToGlobal_),
      ensightGeometryFile
    );
    writeAllPrims
    (
      "tetra4",
      meshCellSets_.nTets,
      map(cellShapes, meshCellSets_.tets, pointToGlobal_),
      ensightGeometryFile
    );
    writeAllPolys
    (
      pointToGlobal_,
      ensightGeometryFile
    );
  }
  label ensightPatchI = patchPartOffset_;
  FOR_ALL(allPatchNames_, patchi) {
    const word& patchName = allPatchNames_[patchi];
    if (!patchNames_.empty() && !patchNames_.found(patchName))
      continue;
    const nFacePrimitives& nfp = nPatchPrims_[patchName];
    if (!nfp.nTris && !nfp.nQuads && !nfp.nPolys)
      continue;
    const polyPatch& p = mesh_.boundaryMesh()[patchi];
    const labelList& tris = boundaryFaceSets_[patchi].tris;
    const labelList& quads = boundaryFaceSets_[patchi].quads;
    const labelList& polys = boundaryFaceSets_[patchi].polys;
    // Renumber the patch points/faces into unique points
    labelList pointToGlobal;
    labelList uniqueMeshPointLabels;
    autoPtr<globalIndex> globalPointsPtr =
      mesh_.globalData().mergePoints
      (
        p.meshPoints(),
        p.meshPointMap(),
        pointToGlobal,
        uniqueMeshPointLabels
      );
    pointField uniquePoints{mesh_.points(), uniqueMeshPointLabels};
    // Renumber the patch faces
    faceList patchFaces{p.localFaces()};
    FOR_ALL(patchFaces, i) {
      inplaceRenumber(pointToGlobal, patchFaces[i]);
    }
    writeAllPoints
    (
      ensightPatchI++,
      patchName,
      uniquePoints,
      globalPointsPtr().size(),
      ensightGeometryFile
    );
    writeAllFacePrims
    (
      "tria3",
      tris,
      nfp.nTris,
      patchFaces,
      ensightGeometryFile
    );
    writeAllFacePrims
    (
      "quad4",
      quads,
      nfp.nQuads,
      patchFaces,
      ensightGeometryFile
    );
    writeAllNSided
    (
      polys,
      nfp.nPolys,
      patchFaces,
      ensightGeometryFile
    );
  }
  // write faceZones, if requested
  FOR_ALL_CONST_ITER(wordHashSet, faceZoneNames_, iter) {
    const word& faceZoneName = iter.key();
    label faceID = mesh_.faceZones().findZoneID(faceZoneName);
    const faceZone& fz = mesh_.faceZones()[faceID];
    const nFacePrimitives& nfp = nFaceZonePrims_[faceZoneName];
    if (!nfp.nTris && !nfp.nQuads && !nfp.nPolys)
      continue;
    const labelList& tris = faceZoneFaceSets_[faceID].tris;
    const labelList& quads = faceZoneFaceSets_[faceID].quads;
    const labelList& polys = faceZoneFaceSets_[faceID].polys;
    // Renumber the faceZone points/faces into unique points
    labelList pointToGlobal;
    labelList uniqueMeshPointLabels;
    autoPtr<globalIndex> globalPointsPtr =
      mesh_.globalData().mergePoints
      (
        fz().meshPoints(),
        fz().meshPointMap(),
        pointToGlobal,
        uniqueMeshPointLabels
      );
    pointField uniquePoints{mesh_.points(), uniqueMeshPointLabels};
    // Find the list of master faces belonging to the faceZone,
    // in local numbering
    faceList faceZoneFaces{fz().localFaces()};
    // Count how many master faces belong to the faceZone. Is there
    // a better way of doing this?
    label nMasterFaces = 0;
    FOR_ALL(fz, faceI) {
      if (faceToBeIncluded(fz[faceI])) {
        ++nMasterFaces;
      }
    }
    // Create the faceList for the master faces only and fill it.
    faceList faceZoneMasterFaces{nMasterFaces};
    label currentFace = 0;
    FOR_ALL(fz, faceI) {
      if (faceToBeIncluded(fz[faceI])) {
        faceZoneMasterFaces[currentFace] = faceZoneFaces[faceI];
        ++currentFace;
      }
    }
    // Renumber the faceZone master faces
    FOR_ALL(faceZoneMasterFaces, i) {
      inplaceRenumber(pointToGlobal, faceZoneMasterFaces[i]);
    }
    writeAllPoints
    (
      ensightPatchI++,
      faceZoneName,
      uniquePoints,
      globalPointsPtr().size(),
      ensightGeometryFile
    );
    writeAllFacePrims
    (
      "tria3",
      tris,
      nfp.nTris,
      faceZoneMasterFaces,
      ensightGeometryFile
    );
    writeAllFacePrims
    (
      "quad4",
      quads,
      nfp.nQuads,
      faceZoneMasterFaces,
      ensightGeometryFile
    );
    writeAllNSided
    (
      polys,
      nfp.nPolys,
      faceZoneMasterFaces,
      ensightGeometryFile
    );
  }
  if (Pstream::master()) {
    delete ensightGeometryFilePtr;
  }
}

