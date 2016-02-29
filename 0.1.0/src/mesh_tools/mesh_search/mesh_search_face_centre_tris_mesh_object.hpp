#ifndef MESH_TOOLS_MESH_SEARCH_MESH_SEARCH_FACE_CENTRE_TRIS_MESH_OBJECT_HPP_
#define MESH_TOOLS_MESH_SEARCH_MESH_SEARCH_FACE_CENTRE_TRIS_MESH_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshSearchFACE_CENTRE_TRISMeshObject
// Description
//   MeshObject wrapper around meshSearch(mesh,  polyMesh::FACE_CENTRE_TRIS).
// SourceFiles
#include "_mesh_object.hpp"
#include "mesh_search.hpp"
namespace mousse
{
class meshSearchFACE_CENTRE_TRISMeshObject
:
  public MeshObject
  <
    polyMesh,
    GeometricMeshObject,
    meshSearchFACE_CENTRE_TRISMeshObject
  >,
  public meshSearch
{
public:
  // Declare name of the class and its debug switch
  TYPE_NAME("meshSearchFACE_CENTRE_TRISMeshObject");
  // Constructors
    //- Constructor given polyMesh
    explicit meshSearchFACE_CENTRE_TRISMeshObject(const polyMesh& mesh);
  //- Destructor
  virtual ~meshSearchFACE_CENTRE_TRISMeshObject()
  {}
};
}  // namespace mousse
#endif
