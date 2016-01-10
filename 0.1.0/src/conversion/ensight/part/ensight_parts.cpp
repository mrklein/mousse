// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_parts.hpp"
// Constructors 
mousse::ensightParts::ensightParts(const polyMesh& mesh)
:
  partsList_()
{
  recalculate(mesh);
}
mousse::ensightParts::ensightParts(const IOobject& ioObj)
:
  partsList_()
{
  IOPtrList<ensightPart> ioList(ioObj);
  partsList_.transfer(ioList);
}
// Destructor 
mousse::ensightParts::~ensightParts()
{}
// Member Functions 
void mousse::ensightParts::recalculate(const polyMesh& mesh)
{
  partsList_.clear();
  // extra space for unzoned cells
  label nPart =
  (
    mesh.cellZones().size()
   + mesh.boundaryMesh().size()
   + 1
  );
  partsList_.setSize(nPart);
  nPart = 0;
  label nZoneCells = 0;
  // do cell zones
  FOR_ALL(mesh.cellZones(), zoneI)
  {
    const cellZone& cZone = mesh.cellZones()[zoneI];
    nZoneCells += cZone.size();
    if (cZone.size())
    {
      partsList_.set
      (
        nPart,
        new ensightPartCells(nPart, mesh, cZone)
      );
      nPart++;
    }
  }
  // collect unzoned cells
  // special case: no zones at all - do entire mesh
  if (nZoneCells == 0)
  {
    partsList_.set
    (
      nPart,
      new ensightPartCells(nPart, mesh)
    );
    nPart++;
  }
  else if (mesh.nCells() > nZoneCells)
  {
    // determine which cells are not in a cellZone
    labelList unzoned(mesh.nCells(), -1);
    FOR_ALL(mesh.cellZones(), zoneI)
    {
      const labelUList& idList = mesh.cellZones()[zoneI];
      FOR_ALL(idList, i)
      {
        unzoned[idList[i]] = idList[i];
      }
    }
    label nUnzoned = 0;
    FOR_ALL(unzoned, i)
    {
      if (unzoned[i] < 0)
      {
        unzoned[nUnzoned] = i;
        nUnzoned++;
      }
    }
    unzoned.setSize(nUnzoned);
    if (unzoned.size())
    {
      partsList_.set
      (
        nPart,
        new ensightPartCells(nPart, mesh, unzoned)
      );
      nPart++;
    }
  }
  // do boundaries, skipping empty and processor patches
  FOR_ALL(mesh.boundaryMesh(), patchI)
  {
    const polyPatch& patch = mesh.boundaryMesh()[patchI];
    if (patch.size() && !isA<processorPolyPatch>(patch))
    {
      partsList_.set
      (
        nPart,
        new ensightPartFaces(nPart, mesh, patch)
      );
      nPart++;
    }
  }
  // truncate to correct size
  partsList_.setSize(nPart);
}
void mousse::ensightParts::renumber
(
  const labelUList& origCellId,
  const labelUList& origFaceId
)
{
  FOR_ALL(partsList_, partI)
  {
    if (partsList_[partI].isCellData())
    {
      partsList_[partI].renumber(origCellId);
    }
    else
    {
      partsList_[partI].renumber(origFaceId);
    }
  }
}
void mousse::ensightParts::writeGeometry(ensightGeoFile& os) const
{
  // with some feedback
  Info<< "write geometry part:" << nl << flush;
  FOR_ALL(partsList_, partI)
  {
    Info<< " " << partI << flush;
    partsList_[partI].writeGeometry(os);
  }
}
bool mousse::ensightParts::writeSummary(Ostream& os) const
{
  FOR_ALL(partsList_, partI)
  {
    partsList_[partI].writeSummary(os);
  }
  return true;
}
void mousse::ensightParts::writeData(Ostream& os) const
{
  // Begin write list
  os  << nl << partsList_.size()
    << nl << token::BEGIN_LIST;
  // Write list contents
  FOR_ALL(partsList_, i)
  {
    os  << nl << partsList_[i];
  }
  // End write list
  os  << nl << token::END_LIST << nl;
  // Check state of IOstream
  os.check("ensightParts::writeData(Ostream&)");
}
void mousse::ensightParts::writeScalarField
(
  ensightFile& os,
  const List<scalar>& field,
  const bool useFaceData,
  const bool perNode
) const
{
  FOR_ALL(partsList_, partI)
  {
    if
    (
      useFaceData
     ? partsList_[partI].isFaceData()
     : partsList_[partI].isCellData()
    )
    {
      partsList_[partI].writeScalarField(os, field, perNode);
    }
  }
}
void mousse::ensightParts::writeVectorField
(
  ensightFile& os,
  const List<scalar>& field0,
  const List<scalar>& field1,
  const List<scalar>& field2,
  const bool useFaceData,
  const bool perNode
) const
{
  FOR_ALL(partsList_, partI)
  {
    if
    (
      useFaceData
     ? partsList_[partI].isFaceData()
     : partsList_[partI].isCellData()
    )
    {
      partsList_[partI].writeVectorField
      (
        os,
        field0, field1, field2,
        perNode
      );
    }
  }
}
//  IOStream operators
mousse::ensightGeoFile& mousse::operator<<
(
  ensightGeoFile& os,
  const ensightParts& parts
)
{
  parts.writeGeometry(os);
  return os;
}
