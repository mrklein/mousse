// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_zone_set.hpp"
#include "map_poly_mesh.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(cellZoneSet, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, cellZoneSet, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, cellZoneSet, size);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, cellZoneSet, set);


// Private Member Functions 
void cellZoneSet::updateSet()
{
  labelList order;
  sortedOrder(addressing_, order);
  inplaceReorder(order, addressing_);
  cellSet::clearStorage();
  cellSet::resize(2*addressing_.size());
  FOR_ALL(addressing_, i) {
    cellSet::insert(addressing_[i]);
  }
}


cellZoneSet::cellZoneSet
(
  const polyMesh& mesh,
  const word& name,
  readOption r,
  writeOption
)
:
  cellSet{mesh, name, 1000},  // do not read cellSet
  mesh_{mesh},
  addressing_{0}
{
  const cellZoneMesh& cellZones = mesh.cellZones();
  label zoneID = cellZones.findZoneID(name);
  if ((r == IOobject::MUST_READ)
      || (r == IOobject::MUST_READ_IF_MODIFIED)
      || (r == IOobject::READ_IF_PRESENT && zoneID != -1)) {
    const cellZone& fz = cellZones[zoneID];
    addressing_ = fz;
  }
  updateSet();
  check(mesh.nCells());
}


cellZoneSet::cellZoneSet
(
  const polyMesh& mesh,
  const word& name,
  const label size,
  writeOption w
)
:
  cellSet{mesh, name, size, w},
  mesh_{mesh},
  addressing_{0}
{
  updateSet();
}


cellZoneSet::cellZoneSet
(
  const polyMesh& mesh,
  const word& name,
  const topoSet& set,
  writeOption w
)
:
  cellSet{mesh, name, set.size(), w},
  mesh_{mesh},
  addressing_{refCast<const cellZoneSet>(set).addressing()}
{
  updateSet();
}


// Destructor 
cellZoneSet::~cellZoneSet()
{}


// Member Functions 
void cellZoneSet::invert(const label maxLen)
{
  // Count
  label n = 0;
  for (label cellI = 0; cellI < maxLen; cellI++) {
    if (!found(cellI)) {
      n++;
    }
  }
  // Fill
  addressing_.setSize(n);
  n = 0;
  for (label cellI = 0; cellI < maxLen; cellI++) {
    if (!found(cellI)) {
      addressing_[n] = cellI;
      n++;
    }
  }
  updateSet();
}


void cellZoneSet::subset(const topoSet& set)
{
  DynamicList<label> newAddressing{addressing_.size()};
  const cellZoneSet& fSet = refCast<const cellZoneSet>(set);
  FOR_ALL(fSet.addressing(), i) {
    label cellI = fSet.addressing()[i];
    if (found(cellI)) {
      newAddressing.append(cellI);
    }
  }
  addressing_.transfer(newAddressing);
  updateSet();
}


void cellZoneSet::addSet(const topoSet& set)
{
  DynamicList<label> newAddressing{addressing_};
  const cellZoneSet& fSet = refCast<const cellZoneSet>(set);
  FOR_ALL(fSet.addressing(), i) {
    label cellI = fSet.addressing()[i];
    if (!found(cellI)) {
      newAddressing.append(cellI);
    }
  }
  addressing_.transfer(newAddressing);
  updateSet();
}


void cellZoneSet::deleteSet(const topoSet& set)
{
  DynamicList<label> newAddressing{addressing_.size()};
  const cellZoneSet& fSet = refCast<const cellZoneSet>(set);
  FOR_ALL(addressing_, i) {
    label cellI = addressing_[i];
    if (!fSet.found(cellI)) {
      // Not found in fSet so add
      newAddressing.append(cellI);
    }
  }
  addressing_.transfer(newAddressing);
  updateSet();
}


void cellZoneSet::sync(const polyMesh&)
{}


label cellZoneSet::maxSize(const polyMesh& mesh) const
{
  return mesh.nCells();
}


//- Write using given format, version and compression
bool cellZoneSet::writeObject
(
  IOstream::streamFormat s,
  IOstream::versionNumber v,
  IOstream::compressionType c
) const
{
  // Write shadow cellSet
  word oldTypeName = typeName;
  const_cast<word&>(type()) = cellSet::typeName;
  bool ok = cellSet::writeObject(s, v, c);
  const_cast<word&>(type()) = oldTypeName;
  // Modify cellZone
  cellZoneMesh& cellZones = const_cast<polyMesh&>(mesh_).cellZones();
  label zoneID = cellZones.findZoneID(name());
  if (zoneID == -1) {
    zoneID = cellZones.size();
    cellZones.setSize(zoneID+1);
    cellZones.set
    (
      zoneID,
      new cellZone
      {
        name(),
        addressing_,
        zoneID,
        cellZones
      }
    );
  } else {
    cellZones[zoneID] = addressing_;
  }
  cellZones.clearAddressing();
  return ok && cellZones.write();
}


void cellZoneSet::updateMesh(const mapPolyMesh& morphMap)
{
  // cellZone
  labelList newAddressing{addressing_.size()};
  label n = 0;
  FOR_ALL(addressing_, i) {
    label cellI = addressing_[i];
    label newCellI = morphMap.reverseCellMap()[cellI];
    if (newCellI >= 0) {
      newAddressing[n] = newCellI;
      n++;
    }
  }
  newAddressing.setSize(n);
  addressing_.transfer(newAddressing);
  updateSet();
}


void cellZoneSet::writeDebug
(
  Ostream& os,
  const primitiveMesh& mesh,
  const label maxLen
) const
{
  cellSet::writeDebug(os, mesh, maxLen);
}

}  // namespace mousse
