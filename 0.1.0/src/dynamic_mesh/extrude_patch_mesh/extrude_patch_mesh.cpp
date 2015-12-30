// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extrude_patch_mesh.hpp"
#include "create_shell_mesh.hpp"
#include "poly_topo_change.hpp"
#include "wall_poly_patch.hpp"
#include "empty_poly_patch.hpp"
#include "wedge_poly_patch.hpp"
namespace mousse
{
// Static Data Members
defineTypeNameAndDebug(extrudePatchMesh, 0);
// Constructors 
extrudePatchMesh::extrudePatchMesh
(
  const fvMesh& mesh,
  const fvPatch& patch,
  const dictionary& dict,
  const word regionName,
  const List<polyPatch*>& regionPatches
)
:
  fvMesh
  (
    IOobject
    (
      regionName,
      mesh.facesInstance(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE,
      true
    ),
    xferCopy(pointField()),
    xferCopy(faceList()),
    xferCopy(labelList()),
    xferCopy(labelList()),
    false
  ),
  extrudedPatch_(patch.patch()),
  dict_(dict)
{
  extrudeMesh(regionPatches);
}
extrudePatchMesh::extrudePatchMesh
(
  const fvMesh& mesh,
  const fvPatch& patch,
  const dictionary& dict,
  const word regionName
)
:
  fvMesh
  (
    IOobject
    (
      regionName,
      mesh.facesInstance(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE,
      true
    ),
    xferCopy(pointField()),
    xferCopy(faceList()),
    xferCopy(labelList()),
    xferCopy(labelList()),
    false
  ),
  extrudedPatch_(patch.patch()),
  dict_(dict)
{
  List<polyPatch*> regionPatches(3);
  List<word> patchNames(regionPatches.size());
  List<word> patchTypes(regionPatches.size());
  PtrList<dictionary> dicts(regionPatches.size());
  forAll (dicts, patchI)
  {
    if (!dicts.set(patchI))
    {
      dicts.set(patchI, new dictionary());
    }
  }
  dicts[bottomPatchID] = dict_.subDict("bottomCoeffs");
  dicts[sidePatchID] = dict_.subDict("sideCoeffs");
  dicts[topPatchID] = dict_.subDict("topCoeffs");
  forAll (dicts, patchI)
  {
    dicts[patchI].lookup("name") >> patchNames[patchI];
    dicts[patchI].lookup("type") >> patchTypes[patchI];
  }
  forAll (regionPatches, patchI)
  {
    dictionary&  patchDict = dicts[patchI];
    patchDict.set("nFaces", 0);
    patchDict.set("startFace", 0);
    regionPatches[patchI] = polyPatch::New
      (
        patchNames[patchI],
        patchDict,
        patchI,
        mesh.boundaryMesh()
      ).ptr();
  }
  extrudeMesh(regionPatches);
}
void extrudePatchMesh::extrudeMesh(const List<polyPatch*>& regionPatches)
{
  if (this->boundaryMesh().size() == 0)
  {
    bool columnCells = readBool(dict_.lookup("columnCells"));
    PackedBoolList nonManifoldEdge(extrudedPatch_.nEdges());
    for (label edgeI = 0; edgeI < extrudedPatch_.nInternalEdges(); edgeI++)
    {
      if (columnCells)
      {
        nonManifoldEdge[edgeI] = true;
      }
    }
    autoPtr<extrudeModel> model_(extrudeModel::New(dict_));
    faceList pointGlobalRegions;
    faceList pointLocalRegions;
    labelList localToGlobalRegion;
    const primitiveFacePatch pp
    (
      extrudedPatch_, extrudedPatch_.points()
    );
    createShellMesh::calcPointRegions
    (
      this->globalData(),
      pp,
      nonManifoldEdge,
      false,
      pointGlobalRegions,
      pointLocalRegions,
      localToGlobalRegion
    );
    // Per local region an originating point
    labelList localRegionPoints(localToGlobalRegion.size());
    forAll(pointLocalRegions, faceI)
    {
      const face& f = extrudedPatch_.localFaces()[faceI];
      const face& pRegions = pointLocalRegions[faceI];
      forAll(pRegions, fp)
      {
        localRegionPoints[pRegions[fp]] = f[fp];
      }
    }
    // Calculate region normals by reducing local region normals
    pointField localRegionNormals(localToGlobalRegion.size());
    {
      pointField localSum(localToGlobalRegion.size(), vector::zero);
      forAll(pointLocalRegions, faceI)
      {
        const face& pRegions = pointLocalRegions[faceI];
        forAll(pRegions, fp)
        {
          label localRegionI = pRegions[fp];
          localSum[localRegionI] +=
            extrudedPatch_.faceNormals()[faceI];
        }
      }
      Map<point> globalSum(2*localToGlobalRegion.size());
      forAll(localSum, localRegionI)
      {
        label globalRegionI = localToGlobalRegion[localRegionI];
        globalSum.insert(globalRegionI, localSum[localRegionI]);
      }
      // Reduce
      Pstream::mapCombineGather(globalSum, plusEqOp<point>());
      Pstream::mapCombineScatter(globalSum);
      forAll(localToGlobalRegion, localRegionI)
      {
        label globalRegionI = localToGlobalRegion[localRegionI];
        localRegionNormals[localRegionI] = globalSum[globalRegionI];
      }
      localRegionNormals /= mag(localRegionNormals);
    }
    // Per local region an extrusion direction
    vectorField firstDisp(localToGlobalRegion.size());
    forAll(firstDisp, regionI)
    {
      //const point& regionPt = regionCentres[regionI];
      const point& regionPt = extrudedPatch_.points()
      [
        extrudedPatch_.meshPoints()
        [
          localRegionPoints[regionI]
        ]
      ];
      const vector& n = localRegionNormals[regionI];
      firstDisp[regionI] = model_()(regionPt, n, 1) - regionPt;
    }
    // Extrude engine
    createShellMesh extruder
    (
      pp,
      pointLocalRegions,
      localRegionPoints
    );
/*
    List<polyPatch*> regionPatches(3);
    List<word> patchNames(regionPatches.size());
    List<word> patchTypes(regionPatches.size());
    PtrList<dictionary> dicts(regionPatches.size());
    forAll (dicts, patchI)
    {
      if (!dicts.set(patchI))
      {
        dicts.set(patchI, new dictionary());
      }
    }
    dicts[bottomPatchID] = dict_.subDict("bottomCoeffs");
    dicts[sidePatchID] = dict_.subDict("sideCoeffs");
    dicts[topPatchID] = dict_.subDict("topCoeffs");
    forAll (dicts, patchI)
    {
      dicts[patchI].lookup("name") >> patchNames[patchI];
      dicts[patchI].lookup("type") >> patchTypes[patchI];
    }
    forAll (regionPatches, patchI)
    {
      dictionary&  patchDict = dicts[patchI];
      patchDict.set("nFaces", 0);
      patchDict.set("startFace", 0);
      regionPatches[patchI] = polyPatch::New
        (
          patchNames[patchI],
          patchDict,
          patchI,
          mesh.boundaryMesh()
        ).ptr();
    }
*/
    this->clearOut();
    this->removeFvBoundary();
    this->addFvPatches(regionPatches, true);
    // At this point we have a valid mesh with 3 patches and zero cells.
    // Determine:
    // - per face the top and bottom patch (topPatchID, bottomPatchID)
    // - per edge, per face on edge the side patch (edgePatches)
    labelListList edgePatches(extrudedPatch_.nEdges());
    forAll(edgePatches, edgeI)
    {
      const labelList& eFaces = extrudedPatch_.edgeFaces()[edgeI];
      if (eFaces.size() != 2 || nonManifoldEdge[edgeI])
      {
        edgePatches[edgeI].setSize(eFaces.size(), sidePatchID);
      }
    }
    polyTopoChange meshMod(regionPatches.size());
    extruder.setRefinement
    (
      firstDisp,                              // first displacement
      model_().expansionRatio(),
      model_().nLayers(),                       // nLayers
      labelList(extrudedPatch_.size(), topPatchID),
      labelList(extrudedPatch_.size(), bottomPatchID),
      edgePatches,
      meshMod
    );
    autoPtr<mapPolyMesh> map = meshMod.changeMesh
    (
      *this,              // mesh to change
      false               // inflate
    );
    // Update numbering on extruder.
    extruder.updateMesh(map);
    this->setInstance(this->thisDb().time().constant());
    this->write();
  }
}
// Destructor 
extrudePatchMesh::~extrudePatchMesh()
{}
// Member Functions 
}  // namespace mousse
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
