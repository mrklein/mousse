// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshSearchMeshObject
// Description
//   MeshObject wrapper around meshSearch(mesh).
// SourceFiles
#ifndef mesh_search_mesh_object_hpp_
#define mesh_search_mesh_object_hpp_
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
