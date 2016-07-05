#ifndef TRI_SURFACE_TRI_SURFACE_GEO_MESH_HPP_
#define TRI_SURFACE_TRI_SURFACE_GEO_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::triSurfaceGeoMesh
// Description
//   The triSurface GeoMesh (for holding fields).
//   Similar to the volGeoMesh used for the Finite Volume discretization.

#include "geo_mesh.hpp"
#include "tri_surface.hpp"


namespace mousse {

class triSurfaceGeoMesh
:
  public GeoMesh<triSurface>
{
public:
  // Constructors
    //- Construct from triSurface reference
    explicit triSurfaceGeoMesh(const triSurface& mesh)
    :
      GeoMesh<triSurface>{mesh}
    {}
  // Member Functions
    //- Return size
    static label size(const triSurface& mesh)
    {
      return mesh.size();
    }
    //- Return size
    label size() const
    {
      return size(mesh_);
    }
};
}  // namespace mousse
#endif
