// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "merge_poly_mesh.hpp"
#include "time.hpp"
#include "poly_topo_changer.hpp"
#include "map_poly_mesh.hpp"
#include "poly_add_point.hpp"
#include "poly_add_cell.hpp"
#include "poly_add_face.hpp"

// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(mergePolyMesh, 1);
}
// Private Member Functions 
mousse::label mousse::mergePolyMesh::patchIndex(const polyPatch& p)
{
  // Find the patch name on the list.  If the patch is already there
  // and patch types match, return index
  const word& pType = p.type();
  const word& pName = p.name();
  bool nameFound = false;
  FOR_ALL(patchNames_, patchI)
  {
    if (patchNames_[patchI] == pName)
    {
      if (word(patchDicts_[patchI]["type"]) == pType)
      {
        // Found name and types match
        return patchI;
      }
      else
      {
        // Found the name, but type is different
        nameFound = true;
      }
    }
  }
  // Patch not found.  Append to the list
  {
    OStringStream os;
    p.write(os);
    patchDicts_.append(dictionary(IStringStream(os.str())()));
  }
  if (nameFound)
  {
    // Duplicate name is not allowed.  Create a composite name from the
    // patch name and case name
    const word& caseName = p.boundaryMesh().mesh().time().caseName();
    patchNames_.append(pName + "_" + caseName);
    Info << "label patchIndex(const polyPatch& p) : "
      << "Patch " << p.index() << " named "
      << pName << " in mesh " << caseName
      << " already exists, but patch types "
      << " do not match.\nCreating a composite name as "
      << patchNames_.last() << endl;
  }
  else
  {
    patchNames_.append(pName);
  }
  return patchNames_.size() - 1;
}
mousse::label mousse::mergePolyMesh::zoneIndex
(
  DynamicList<word>& names,
  const word& curName
)
{
  FOR_ALL(names, zoneI)
  {
    if (names[zoneI] == curName)
    {
      return zoneI;
    }
  }
  // Not found.  Add new name to the list
  names.append(curName);
  return names.size() - 1;
}
// Constructors 
mousse::mergePolyMesh::mergePolyMesh(const IOobject& io)
:
  polyMesh{io},
  meshMod_{*this},
  patchNames_{2*boundaryMesh().size()},
  patchDicts_{2*boundaryMesh().size()},
  pointZoneNames_{},
  faceZoneNames_{},
  cellZoneNames_{}
{
  // Insert the original patches into the list
  wordList curPatchNames = boundaryMesh().names();
  FOR_ALL(boundaryMesh(), patchI)
  {
    patchNames_.append(boundaryMesh()[patchI].name());
    OStringStream os;
    boundaryMesh()[patchI].write(os);
    patchDicts_.append(dictionary(IStringStream(os.str())()));
  }
  // Insert point, face and cell zones into the list
  // Point zones
  wordList curPointZoneNames = pointZones().names();
  if (curPointZoneNames.size())
  {
    pointZoneNames_.setCapacity(2*curPointZoneNames.size());
  }
  FOR_ALL(curPointZoneNames, zoneI)
  {
    pointZoneNames_.append(curPointZoneNames[zoneI]);
  }
  // Face zones
  wordList curFaceZoneNames = faceZones().names();
  if (curFaceZoneNames.size())
  {
    faceZoneNames_.setCapacity(2*curFaceZoneNames.size());
  }
  FOR_ALL(curFaceZoneNames, zoneI)
  {
    faceZoneNames_.append(curFaceZoneNames[zoneI]);
  }
  // Cell zones
  wordList curCellZoneNames = cellZones().names();
  if (curCellZoneNames.size())
  {
    cellZoneNames_.setCapacity(2*curCellZoneNames.size());
  }
  FOR_ALL(curCellZoneNames, zoneI)
  {
    cellZoneNames_.append(curCellZoneNames[zoneI]);
  }
}
// Destructor 
// Member Functions 
void mousse::mergePolyMesh::addMesh(const polyMesh& m)
{
  // Add all the points, faces and cells of the new mesh
  // Add points
  label zoneID = -1;
  const pointField& p = m.points();
  labelList renumberPoints(p.size());
  const pointZoneMesh& pz = m.pointZones();
  labelList pointZoneIndices(pz.size());
  FOR_ALL(pz, zoneI)
  {
    pointZoneIndices[zoneI] = zoneIndex(pointZoneNames_, pz[zoneI].name());
  }
  FOR_ALL(p, pointI)
  {
    // Grab zone ID.  If a point is not in a zone, it will return -1
    zoneID = pz.whichZone(pointI);
    if (zoneID >= 0)
    {
      // Translate zone ID into the new index
      zoneID = pointZoneIndices[zoneID];
    }
    renumberPoints[pointI] =
      meshMod_.setAction
      (
        polyAddPoint
        {
          p[pointI],            // Point to add
          -1,                   // Master point (straight addition)
          zoneID,               // Zone for point
          pointI < m.nPoints()  // Is in cell?
        }
      );
  }
  // Add cells
  const cellList& c = m.cells();
  labelList renumberCells{c.size()};
  const cellZoneMesh& cz = m.cellZones();
  labelList cellZoneIndices{cz.size()};
  FOR_ALL(cz, zoneI)
  {
    cellZoneIndices[zoneI] = zoneIndex(cellZoneNames_, cz[zoneI].name());
  }
  FOR_ALL(c, cellI)
  {
    // Grab zone ID.  If a cell is not in a zone, it will return -1
    zoneID = cz.whichZone(cellI);
    if (zoneID >= 0)
    {
      // Translate zone ID into the new index
      zoneID = cellZoneIndices[zoneID];
    }
    renumberCells[cellI] =
      meshMod_.setAction
      (
        polyAddCell
        {
          -1,                   // Master point
          -1,                   // Master edge
          -1,                   // Master face
          -1,                   // Master cell
          zoneID                // Zone for cell
        }
      );
  }
  // Add faces
  const polyBoundaryMesh& bm = m.boundaryMesh();
  // Gather the patch indices
  labelList patchIndices{bm.size()};
  FOR_ALL(patchIndices, patchI)
  {
    patchIndices[patchI] = patchIndex(bm[patchI]);
  }
  // Temporary: update number of allowable patches. This should be
  // determined at the top - before adding anything.
  meshMod_.setNumPatches(patchNames_.size());
  const faceZoneMesh& fz = m.faceZones();
  labelList faceZoneIndices{fz.size()};
  FOR_ALL(fz, zoneI)
  {
    faceZoneIndices[zoneI] = zoneIndex(faceZoneNames_, fz[zoneI].name());
  }
  const faceList& f = m.faces();
  labelList renumberFaces{f.size()};
  const labelList& own = m.faceOwner();
  const labelList& nei = m.faceNeighbour();
  label newOwn, newNei, newPatch, newZone;
  bool newZoneFlip;
  FOR_ALL(f, faceI)
  {
    const face& curFace = f[faceI];
    face newFace(curFace.size());
    FOR_ALL(curFace, pointI)
    {
      newFace[pointI] = renumberPoints[curFace[pointI]];
    }
    if (debug)
    {
      // Check that the face is valid
      if (min(newFace) < 0)
      {
        FATAL_ERROR_IN("void mergePolyMesh::addMesh(const polyMesh&)")
          << "Error in point mapping for face " << faceI
          << ".  Old face: " << curFace << " New face: " << newFace
          << abort(FatalError);
      }
    }
    if (faceI < m.nInternalFaces() || faceI >= m.nFaces())
    {
      newPatch = -1;
    }
    else
    {
      newPatch = patchIndices[bm.whichPatch(faceI)];
    }
    newOwn = own[faceI];
    if (newOwn > -1) newOwn = renumberCells[newOwn];
    if (newPatch > -1)
    {
      newNei = -1;
    }
    else
    {
      newNei = nei[faceI];
      newNei = renumberCells[newNei];
    }
    newZone = fz.whichZone(faceI);
    newZoneFlip = false;
    if (newZone >= 0)
    {
      newZoneFlip = fz[newZone].flipMap()[fz[newZone].whichFace(faceI)];
      // Grab the new zone
      newZone = faceZoneIndices[newZone];
    }
    renumberFaces[faceI] =
      meshMod_.setAction
      (
        polyAddFace
        {
          newFace,
          newOwn,
          newNei,
          -1,
          -1,
          -1,
          false,
          newPatch,
          newZone,
          newZoneFlip
        }
      );
  }
}
void mousse::mergePolyMesh::merge()
{
  Info << "patch names: " << patchNames_ << nl
    << "patch dicts: " << patchDicts_ << nl
    << "point zone names: " << pointZoneNames_ << nl
    << "face zone names: " << faceZoneNames_ << nl
    << "cell zone names: " << cellZoneNames_ << endl;
  // Add the patches if necessary
  if (patchNames_.size() != boundaryMesh().size())
  {
    Info << "Copying old patches" << endl;
    List<polyPatch*> newPatches{patchNames_.size()};
    const polyBoundaryMesh& oldPatches = boundaryMesh();
    // Note.  Re-using counter in two for loops
    label patchI = 0;
    for (patchI = 0; patchI < oldPatches.size(); patchI++)
    {
      newPatches[patchI] = oldPatches[patchI].clone(oldPatches).ptr();
    }
    Info << "Adding new patches. " << endl;
    label endOfLastPatch =
      oldPatches[patchI - 1].start() + oldPatches[patchI - 1].size();
    for (; patchI < patchNames_.size(); patchI++)
    {
      // Add a patch
      dictionary dict{patchDicts_[patchI]};
      dict.set("nFaces", 0);
      dict.set("startFace", endOfLastPatch);
      newPatches[patchI] = polyPatch::New(patchNames_[patchI],
                                          dict,
                                          patchI,
                                          oldPatches
                                         ).ptr();
    }
    removeBoundary();
    addPatches(newPatches);
  }
  // Add the zones if necessary
  if (pointZoneNames_.size() > pointZones().size())
  {
    Info << "Adding new pointZones. " << endl;
    label nZones = pointZones().size();
    pointZones().setSize(pointZoneNames_.size());
    for (label zoneI = nZones; zoneI < pointZoneNames_.size(); zoneI++)
    {
      pointZones().set
      (
        zoneI,
        new pointZone
        {
          pointZoneNames_[zoneI],
          labelList(),
          zoneI,
          pointZones()
        }
      );
    }
  }
  if (cellZoneNames_.size() > cellZones().size())
  {
    Info << "Adding new cellZones. " << endl;
    label nZones = cellZones().size();
    cellZones().setSize(cellZoneNames_.size());
    for (label zoneI = nZones; zoneI < cellZoneNames_.size(); zoneI++)
    {
      cellZones().set
      (
        zoneI,
        new cellZone
        {
          cellZoneNames_[zoneI],
          labelList(),
          zoneI,
          cellZones()
        }
      );
    }
  }
  if (faceZoneNames_.size() > faceZones().size())
  {
    Info << "Adding new faceZones. " << endl;
    label nZones = faceZones().size();
    faceZones().setSize(faceZoneNames_.size());
    for (label zoneI = nZones; zoneI < faceZoneNames_.size(); zoneI++)
    {
      faceZones().set
      (
        zoneI,
        new faceZone
        {
          faceZoneNames_[zoneI],
          labelList(),
          boolList(),
          zoneI,
          faceZones()
        }
      );
    }
  }
  // Change mesh. No inflation
  meshMod_.changeMesh(*this, false);
  // Clear topo change for the next operation
  meshMod_.clear();
}
