// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfPointGeoMesh
// Description
//   The surfMesh GeoMesh (for holding fields).
//   Similar to surfGeoMesh, but refers to the surface points.
#ifndef surf_point_geo_mesh_hpp_
#define surf_point_geo_mesh_hpp_
#include "geo_mesh.hpp"
#include "surf_mesh.hpp"
namespace mousse
{
class surfPointGeoMesh
:
  public GeoMesh<surfMesh>
{
public:
  // Constructors
    //- Construct from surfMesh reference
    explicit surfPointGeoMesh(const surfMesh& mesh)
    :
      GeoMesh<surfMesh>(mesh)
    {}
  // Member Functions
    //- Return size
    static label size(const surfMesh& mesh)
    {
      return mesh.nPoints();
    }
    //- Return size
    label size() const
    {
      return size(mesh_);
    }
};
}  // namespace mousse
#endif
