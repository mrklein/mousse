// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_set.hpp"
#include "map_poly_mesh.hpp"
#include "poly_mesh.hpp"
#include "sync_tools.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(pointSet, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, pointSet, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, pointSet, size);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, pointSet, set);


pointSet::pointSet(const IOobject& obj)
:
  topoSet{obj, typeName}
{}


pointSet::pointSet
(
  const polyMesh& mesh,
  const word& name,
  readOption r,
  writeOption w
)
:
  topoSet{mesh, typeName, name, r, w}
{
  check(mesh.nPoints());
}


pointSet::pointSet
(
  const polyMesh& mesh,
  const word& name,
  const label size,
  writeOption w
)
:
  topoSet{mesh, name, size, w}
{}


pointSet::pointSet
(
  const polyMesh& mesh,
  const word& name,
  const topoSet& set,
  writeOption w
)
:
  topoSet{mesh, name, set, w}
{}


pointSet::pointSet
(
  const polyMesh& mesh,
  const word& name,
  const labelHashSet& set,
  writeOption w
)
:
  topoSet{mesh, name, set, w}
{}


// Destructor 
pointSet::~pointSet()
{}


// Member Functions 
void pointSet::sync(const polyMesh& mesh)
{
  // Convert to boolList
  boolList contents{mesh.nPoints(), false};
  FOR_ALL_CONST_ITER(pointSet, *this, iter) {
    contents[iter.key()] = true;
  }
  syncTools::syncPointList
  (
    mesh,
    contents,
    orEqOp<bool>(),
    false           // null value
  );
  // Convert back to labelHashSet
  labelHashSet newContents(size());
  FOR_ALL(contents, pointI) {
    if (contents[pointI]) {
      newContents.insert(pointI);
    }
  }
  transfer(newContents);
}


label pointSet::maxSize(const polyMesh& mesh) const
{
  return mesh.nPoints();
}


void pointSet::updateMesh(const mapPolyMesh& morphMap)
{
  updateLabels(morphMap.reversePointMap());
}


void pointSet::writeDebug
(
  Ostream& os,
  const primitiveMesh& mesh,
  const label maxLen
) const
{
  topoSet::writeDebug(os, mesh.points(), maxLen);
}

}  // namespace mousse

