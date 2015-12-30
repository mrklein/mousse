// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfGeoMesh
// Description
//   The surfMesh GeoMesh (for holding fields).
//   Similar to the volGeoMesh used for the Finite Volume discretization.
#ifndef surf_geo_mesh_hpp_
#define surf_geo_mesh_hpp_
#include "geo_mesh.hpp"
#include "surf_mesh.hpp"
namespace mousse
{
class surfGeoMesh
:
  public GeoMesh<surfMesh>
{
public:
  // Constructors
    //- Construct from surfMesh reference
    explicit surfGeoMesh(const surfMesh& mesh)
    :
      GeoMesh<surfMesh>(mesh)
    {}
  // Member Functions
    //- Return size
    static label size(const surfMesh& mesh)
    {
      return mesh.nFaces();
    }
    //- Return size
    label size() const
    {
      return size(mesh_);
    }
};
}  // namespace mousse
#endif
