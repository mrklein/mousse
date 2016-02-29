#ifndef FINITE_VOLUME_VOL_MESH_HPP_
#define FINITE_VOLUME_VOL_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::volMesh
// Description
//   Mesh data needed to do the Finite Volume discretisation.
#include "geo_mesh.hpp"
#include "fv_mesh.hpp"
#include "primitive_mesh.hpp"
namespace mousse
{
class volMesh
:
  public GeoMesh<fvMesh>
{
public:
  // Constructors
    //- Construct from fvMesh
    explicit volMesh(const fvMesh& mesh)
    :
      GeoMesh<fvMesh>(mesh)
    {}
  // Member Functions
    //- Return size
    label size() const
    {
      return size(mesh_);
    }
    //- Return size
    static label size(const Mesh& mesh)
    {
      return mesh.nCells();
    }
    //- Return cell centres
    const volVectorField& C()
    {
      return mesh_.C();
    }
};
}  // namespace mousse
#endif
