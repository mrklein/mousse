#ifndef MESH_TOOLS_MESH_SEARCH_MESH_SEARCH_MESH_OBJECT_HPP_
#define MESH_TOOLS_MESH_SEARCH_MESH_SEARCH_MESH_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshSearchMeshObject
// Description
//   MeshObject wrapper around meshSearch(mesh).
// SourceFiles
#include "_mesh_object.hpp"
#include "mesh_search.hpp"
namespace mousse
{
class meshSearchMeshObject
:
  public MeshObject<polyMesh, GeometricMeshObject, meshSearchMeshObject>,
  public meshSearch
{
public:
  // Declare name of the class and its debug switch
  TYPE_NAME("meshSearchMeshObject");
  // Constructors
    //- Constructor given polyMesh
    explicit meshSearchMeshObject(const polyMesh& mesh);
  //- Destructor
  virtual ~meshSearchMeshObject()
  {}
};
}  // namespace mousse
#endif
