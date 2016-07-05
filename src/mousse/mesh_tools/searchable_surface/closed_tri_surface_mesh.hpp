#ifndef MESH_TOOLS_SEARCHABLE_SURFACE_CLOSED_TRI_SURFACE_MESH_HPP_
#define MESH_TOOLS_SEARCHABLE_SURFACE_CLOSED_TRI_SURFACE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::closedTriSurfaceMesh
// Description
//   A triSurfaceMesh where it is forced to check volumeTypes, used for surfaces
//   that are topologically non-manifold (small holes or multiple parts) but are
//   geometrically essentially closed

#include "tri_surface_mesh.hpp"


namespace mousse {

class closedTriSurfaceMesh
:
  public triSurfaceMesh
{
public:
  //- Runtime type information
  TYPE_NAME("closedTriSurfaceMesh");
  // Constructors
    //- Construct from triSurface
    closedTriSurfaceMesh(const IOobject&, const triSurface&);
    //- Construct read.
    closedTriSurfaceMesh(const IOobject& io);
    //- Construct from IO and dictionary (used by searchableSurface).
    //  Dictionary may contain a 'scale' entry (eg, 0.001: mm -> m)
    closedTriSurfaceMesh
    (
      const IOobject& io,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    closedTriSurfaceMesh(const closedTriSurfaceMesh&) = delete;
    //- Disallow default bitwise assignment
    closedTriSurfaceMesh& operator=(const closedTriSurfaceMesh&) = delete;
  // Destructor
    virtual ~closedTriSurfaceMesh();
  // Member Functions
    //- Whether supports volume type, forcing to true to force getVolumeType
    //  queries for this type
    virtual bool hasVolumeType() const
    {
      return true;
    }
};
}  // namespace mousse
#endif
