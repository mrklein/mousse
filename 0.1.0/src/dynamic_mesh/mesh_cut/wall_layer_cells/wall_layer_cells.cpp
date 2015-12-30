// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_layer_cells.hpp"
#include "dynamic_list.hpp"
#include "mesh_wave.hpp"
#include "wall_normal_info.hpp"
#include "ofstream.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(wallLayerCells, 0);
}
// Private Member Functions 
bool mousse::wallLayerCells::usesCoupledPatch(const label cellI) const
{
  const polyBoundaryMesh& patches = mesh().boundaryMesh();
  const cell& cFaces = mesh().cells()[cellI];
  forAll(cFaces, cFaceI)
  {
    label faceI = cFaces[cFaceI];
    label patchID = patches.whichPatch(faceI);
    if ((patchID >= 0) && (patches[patchID].coupled()))
    {
      return true;
    }
  }
  return false;
}
// Constructors 
// Construct from components
mousse::wallLayerCells::wallLayerCells
(
  const polyMesh& mesh,
  const List<word>& patchNames,
  const label nLayers
)
:
  edgeVertex(mesh),
  List<refineCell>()
{
  // Find out cells connected to walls.
  const polyPatchList& patches = mesh.boundaryMesh();
  // Make map from name to local patch ID
  HashTable<label> patchNameToIndex(patches.size());
  forAll(patches, patchI)
  {
    patchNameToIndex.insert(patches[patchI].name(), patchI);
  }
  // Count size of walls to set
  label nWalls = 0;
  forAll(patchNames, patchNameI)
  {
    const word& name = patchNames[patchNameI];
    if (patchNameToIndex.found(name))
    {
      label patchI = patchNameToIndex[name];
      nWalls += patches[patchI].size();
    }
  }
  // Allocate storage for start of wave on faces
  List<wallNormalInfo> changedFacesInfo(nWalls);
  labelList changedFaces(nWalls);
  // Fill changedFaces info
  label nChangedFaces = 0;
  forAll(patchNames, patchNameI)
  {
    const word& name = patchNames[patchNameI];
    if (patchNameToIndex.found(name))
    {
      label patchI = patchNameToIndex[name];
      const polyPatch& pp = patches[patchI];
      forAll(pp, patchFaceI)
      {
        label meshFaceI = pp.start() + patchFaceI;
        changedFaces[nChangedFaces] = meshFaceI;
        // Set transported information to the wall normal.
        const vector& norm = pp.faceNormals()[patchFaceI];
        changedFacesInfo[nChangedFaces] = wallNormalInfo(norm);
        nChangedFaces++;
      }
    }
  }
  // Do a wave of nLayers, transporting the index in patchNames
  // (cannot use local patchIDs since we might get info from neighbouring
  //  processor)
  MeshWave<wallNormalInfo> regionCalc
  (
    mesh,
    changedFaces,
    changedFacesInfo,
    0
  );
  regionCalc.iterate(nLayers);
  // Now regionCalc should hold info on faces that are reachable from
  // changedFaces within nLayers iterations. We use face info since that is
  // guaranteed to be consistent across processor boundaries.
  const List<wallNormalInfo>& faceInfo = regionCalc.allFaceInfo();
  if (debug)
  {
    Info<< "wallLayerCells::getRefinement : dumping selected faces to "
      << "selectedFaces.obj" << endl;
    OFstream fcStream("selectedFaces.obj");
    label vertI = 0;
    forAll(faceInfo, faceI)
    {
      const wallNormalInfo& info = faceInfo[faceI];
      if (info.valid(regionCalc.data()))
      {
        const face& f = mesh.faces()[faceI];
        point mid(0.0, 0.0, 0.0);
        forAll(f, fp)
        {
          mid += mesh.points()[f[fp]];
        }
        mid /= f.size();
        fcStream
          << "v " << mid.x() << ' ' << mid.y() << ' ' << mid.z()
          << endl;
        vertI++;
        point end(mid + info.normal());
        fcStream
          << "v " << end.x() << ' ' << end.y() << ' ' << end.z()
          << endl;
        vertI++;
        fcStream << "l " << vertI << ' ' <<vertI-1 << endl;
      }
    }
  }
  //
  // Copy meshWave information to List<refineCell>
  //
  // Estimate size
  DynamicList<refineCell> refineCells(3*nWalls);
  const List<wallNormalInfo>& cellInfo = regionCalc.allCellInfo();
  forAll(cellInfo, cellI)
  {
    const wallNormalInfo& info = cellInfo[cellI];
    if (info.valid(regionCalc.data()) && !usesCoupledPatch(cellI))
    {
      refineCells.append(refineCell(cellI, info.normal()));
    }
  }
  // Transfer refineCells storage to this.
  transfer(refineCells);
}
