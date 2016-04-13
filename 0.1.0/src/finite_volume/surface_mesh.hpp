#ifndef FINITE_VOLUME_SURFACE_MESH_HPP_
#define FINITE_VOLUME_SURFACE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceMesh
// Description
//   Mesh data needed to do the Finite Volume discretisation.

#include "geo_mesh.hpp"
#include "fv_mesh.hpp"
#include "primitive_mesh.hpp"


namespace mousse {

class surfaceMesh
:
  public GeoMesh<fvMesh>
{
public:
  explicit surfaceMesh(const fvMesh& mesh)
  :
    GeoMesh<fvMesh>{mesh}
  {}
  label size() const
  {
    return size(mesh_);
  }
  static label size(const Mesh& mesh)
  {
    return mesh.nInternalFaces();
  }
  const surfaceVectorField& C()
  {
    return mesh_.Cf();
  }
};
}  // namespace mousse
#endif
