#ifndef CORE_MESHES_GEO_MESH_HPP_
#define CORE_MESHES_GEO_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GeoMesh
// Description
//   Generic mesh wrapper used by volMesh, surfaceMesh, pointMesh etc.
#include "object_registry.hpp"
namespace mousse
{
template<class MESH>
class GeoMesh
{
protected:
  // Protected data
    //- Reference to Mesh
    const MESH& mesh_;
public:
  // Public typedefs
    typedef MESH Mesh;
    typedef typename MESH::BoundaryMesh BoundaryMesh;
  // Constructors
    //- Construct from MESH
    explicit GeoMesh(const MESH& mesh)
    :
      mesh_(mesh)
    {}
  // Member Functions
    //- Return the object registry
    const objectRegistry& thisDb() const
    {
      return mesh_;
    }
  // Member Operators
    //- Return reference to polyMesh
    const MESH& operator()() const
    {
      return mesh_;
    }
};
}  // namespace mousse
#endif
