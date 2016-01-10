// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_set.hpp"
#include "map_poly_mesh.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(cellSet, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, cellSet, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, cellSet, size);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSet, cellSet, set);
cellSet::cellSet(const IOobject& obj)
:
  topoSet(obj, typeName)
{}
cellSet::cellSet
(
  const polyMesh& mesh,
  const word& name,
  readOption r,
  writeOption w
)
:
  topoSet(mesh, typeName, name, r, w)
{
  // Make sure set within valid range
  check(mesh.nCells());
}
cellSet::cellSet
(
  const polyMesh& mesh,
  const word& name,
  const label size,
  writeOption w
)
:
  topoSet(mesh, name, size, w)
{}
cellSet::cellSet
(
  const polyMesh& mesh,
  const word& name,
  const topoSet& set,
  writeOption w
)
:
  topoSet(mesh, name, set, w)
{}
cellSet::cellSet
(
  const polyMesh& mesh,
  const word& name,
  const labelHashSet& set,
  writeOption w
)
:
  topoSet(mesh, name, set, w)
{}
// Database constructors (for when no mesh available)
cellSet::cellSet
(
  const Time& runTime,
  const word& name,
  readOption r,
  writeOption w
)
:
  topoSet
  (
    IOobject
    (
      name,
      runTime.findInstance
      (
        polyMesh::meshSubDir/"sets",    //polyMesh::meshSubDir,
        word::null,                     //"faces"
        IOobject::MUST_READ,
        runTime.findInstance
        (
          polyMesh::meshSubDir,
          "faces",
          IOobject::READ_IF_PRESENT
        )
      ),
      polyMesh::meshSubDir/"sets",
      runTime,
      r,
      w
    ),
    typeName
  )
{}
cellSet::cellSet
(
  const Time& runTime,
  const word& name,
  const label size,
  writeOption w
)
:
  topoSet
  (
    IOobject
    (
      name,
      runTime.findInstance
      (
        polyMesh::meshSubDir/"sets",    //polyMesh::meshSubDir,
        word::null,                     //"faces"
        IOobject::NO_READ,
        runTime.findInstance
        (
          polyMesh::meshSubDir,
          "faces",
          IOobject::READ_IF_PRESENT
        )
      ),
      polyMesh::meshSubDir/"sets",
      runTime,
      IOobject::NO_READ,
      w
    ),
    size
  )
{}
cellSet::cellSet
(
  const Time& runTime,
  const word& name,
  const labelHashSet& set,
  writeOption w
)
:
  topoSet
  (
    IOobject
    (
      name,
      runTime.findInstance
      (
        polyMesh::meshSubDir/"sets",    //polyMesh::meshSubDir,
        word::null,                     //"faces"
        IOobject::NO_READ,
        runTime.findInstance
        (
          polyMesh::meshSubDir,
          "faces",
          IOobject::READ_IF_PRESENT
        )
      ),
      polyMesh::meshSubDir/"sets",
      runTime,
      IOobject::NO_READ,
      w
    ),
    set
  )
{}
// Destructor 
cellSet::~cellSet()
{}
// Member Functions 
label cellSet::maxSize(const polyMesh& mesh) const
{
  return mesh.nCells();
}
void cellSet::updateMesh(const mapPolyMesh& morphMap)
{
  updateLabels(morphMap.reverseCellMap());
}
void mousse::cellSet::writeDebug
(
  Ostream& os,
  const primitiveMesh& mesh,
  const label maxLen
) const
{
  topoSet::writeDebug(os, mesh.cellCentres(), maxLen);
}
}  // namespace mousse
