// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::triSurfaceGeoMesh
// Description
//   The triSurface  point GeoMesh (for holding vertex fields).
//   Similar to the volMesh used for the Finite Volume discretization.
#ifndef tri_surface_point_geo_mesh_hpp_
#define tri_surface_point_geo_mesh_hpp_
#include "geo_mesh.hpp"
#include "tri_surface.hpp"
namespace mousse
{
class triSurfacePointGeoMesh
:
  public GeoMesh<triSurface>
{
public:
  // Constructors
    //- Construct from triSurface reference
    explicit triSurfacePointGeoMesh(const triSurface& mesh)
    :
      GeoMesh<triSurface>(mesh)
    {}
  // Member Functions
    //- Return size
    static label size(const triSurface& mesh)
    {
      return mesh.points().size();
    }
    //- Return size
    label size() const
    {
      return size(mesh_);
    }
};
}  // namespace mousse
#endif
