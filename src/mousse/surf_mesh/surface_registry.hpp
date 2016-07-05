#ifndef SURF_MESH_SURFACE_REGISTRY_SURFACE_REGISTRY_HPP_
#define SURF_MESH_SURFACE_REGISTRY_SURFACE_REGISTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceRegistry
// Description
//   Wraps the normal objectRegistry with a local instance for %surfaces.

#include "object_registry.hpp"


namespace mousse {

class surfaceRegistry
:
  public objectRegistry
{
public:
  //- Runtime type information
  TYPE_NAME("surfaceRegistry");
    //- The prefix to local: %surfaces
    static const word prefix;
    //- The default surface name: %default
    static word defaultName;
  // Constructors
    //- Construct for the given objectRegistry and named surface
    surfaceRegistry(const objectRegistry&, const word& surfName = "");
    //- Disallow default bitwise copy construct
    surfaceRegistry(const surfaceRegistry&) = delete;
    //- Disallow default bitwise assignment
    surfaceRegistry& operator=(const surfaceRegistry&) = delete;
  //- Destructor
  virtual ~surfaceRegistry();
};
}  // namespace mousse
#endif
