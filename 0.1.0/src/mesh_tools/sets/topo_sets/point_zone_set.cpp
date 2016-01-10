// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_zone_set.hpp"
#include "map_poly_mesh.hpp"
#include "poly_mesh.hpp"
#include "processor_poly_patch.hpp"
#include "cyclic_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(pointZoneSet, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, pointZoneSet, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, pointZoneSet, size);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, pointZoneSet, set);

// Private Member Functions 
void pointZoneSet::updateSet()
{
  labelList order;
  sortedOrder(addressing_, order);
  inplaceReorder(order, addressing_);
  pointSet::clearStorage();
  pointSet::resize(2*addressing_.size());
  FOR_ALL(addressing_, i)
  {
    pointSet::insert(addressing_[i]);
  }
}
pointZoneSet::pointZoneSet
(
  const polyMesh& mesh,
  const word& name,
  readOption r,
  writeOption
)
:
  pointSet(mesh, name, 1000),  // do not read pointSet
  mesh_(mesh),
  addressing_(0)
{
  const pointZoneMesh& pointZones = mesh.pointZones();
  label zoneID = pointZones.findZoneID(name);
  if
  (
    r == IOobject::MUST_READ
  || r == IOobject::MUST_READ_IF_MODIFIED
  || (r == IOobject::READ_IF_PRESENT && zoneID != -1)
  )
  {
    const pointZone& fz = pointZones[zoneID];
    addressing_ = fz;
  }
  updateSet();
  check(mesh.nPoints());
}
pointZoneSet::pointZoneSet
(
  const polyMesh& mesh,
  const word& name,
  const label size,
  writeOption w
)
:
  pointSet(mesh, name, size, w),
  mesh_(mesh),
  addressing_(0)
{
  updateSet();
}
pointZoneSet::pointZoneSet
(
  const polyMesh& mesh,
  const word& name,
  const topoSet& set,
  writeOption w
)
:
  pointSet(mesh, name, set.size(), w),
  mesh_(mesh),
  addressing_(refCast<const pointZoneSet>(set).addressing())
{
  updateSet();
}
// Destructor 
pointZoneSet::~pointZoneSet()
{}
// Member Functions 
void pointZoneSet::invert(const label maxLen)
{
  // Count
  label n = 0;
  for (label pointI = 0; pointI < maxLen; pointI++)
  {
    if (!found(pointI))
    {
      n++;
    }
  }
  // Fill
  addressing_.setSize(n);
  n = 0;
  for (label pointI = 0; pointI < maxLen; pointI++)
  {
    if (!found(pointI))
    {
      addressing_[n] = pointI;
      n++;
    }
  }
  updateSet();
}
void pointZoneSet::subset(const topoSet& set)
{
  DynamicList<label> newAddressing(addressing_.size());
  const pointZoneSet& fSet = refCast<const pointZoneSet>(set);
  FOR_ALL(fSet.addressing(), i)
  {
    label pointI = fSet.addressing()[i];
    if (found(pointI))
    {
      newAddressing.append(pointI);
    }
  }
  addressing_.transfer(newAddressing);
  updateSet();
}
void pointZoneSet::addSet(const topoSet& set)
{
  DynamicList<label> newAddressing(addressing_);
  const pointZoneSet& fSet = refCast<const pointZoneSet>(set);
  FOR_ALL(fSet.addressing(), i)
  {
    label pointI = fSet.addressing()[i];
    if (!found(pointI))
    {
      newAddressing.append(pointI);
    }
  }
  addressing_.transfer(newAddressing);
  updateSet();
}
void pointZoneSet::deleteSet(const topoSet& set)
{
  DynamicList<label> newAddressing(addressing_.size());
  const pointZoneSet& fSet = refCast<const pointZoneSet>(set);
  FOR_ALL(addressing_, i)
  {
    label pointI = addressing_[i];
    if (!fSet.found(pointI))
    {
      // Not found in fSet so add
      newAddressing.append(pointI);
    }
  }
  addressing_.transfer(newAddressing);
  updateSet();
}
void pointZoneSet::sync(const polyMesh&)
{}
label pointZoneSet::maxSize(const polyMesh& mesh) const
{
  return mesh.nPoints();
}
//- Write using given format, version and compression
bool pointZoneSet::writeObject
(
  IOstream::streamFormat s,
  IOstream::versionNumber v,
  IOstream::compressionType c
) const
{
  // Write shadow pointSet
  word oldTypeName = typeName;
  const_cast<word&>(type()) = pointSet::typeName;
  bool ok = pointSet::writeObject(s, v, c);
  const_cast<word&>(type()) = oldTypeName;
  // Modify pointZone
  pointZoneMesh& pointZones = const_cast<polyMesh&>(mesh_).pointZones();
  label zoneID = pointZones.findZoneID(name());
  if (zoneID == -1)
  {
    zoneID = pointZones.size();
    pointZones.setSize(zoneID+1);
    pointZones.set
    (
      zoneID,
      new pointZone
      (
        name(),
        addressing_,
        zoneID,
        pointZones
      )
    );
  }
  else
  {
    pointZones[zoneID] = addressing_;
  }
  pointZones.clearAddressing();
  return ok && pointZones.write();
}
void pointZoneSet::updateMesh(const mapPolyMesh& morphMap)
{
  // pointZone
  labelList newAddressing(addressing_.size());
  label n = 0;
  FOR_ALL(addressing_, i)
  {
    label pointI = addressing_[i];
    label newPointI = morphMap.reversePointMap()[pointI];
    if (newPointI >= 0)
    {
      newAddressing[n] = newPointI;
      n++;
    }
  }
  newAddressing.setSize(n);
  addressing_.transfer(newAddressing);
  updateSet();
}
void pointZoneSet::writeDebug
(
  Ostream& os,
  const primitiveMesh& mesh,
  const label maxLen
) const
{
  pointSet::writeDebug(os, mesh, maxLen);
}
}  // namespace mousse
