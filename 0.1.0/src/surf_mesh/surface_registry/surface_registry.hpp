// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceRegistry
// Description
//   Wraps the normal objectRegistry with a local instance for %surfaces.
// SourceFiles
//   surface_registry.cpp
#ifndef surface_registry_hpp_
#define surface_registry_hpp_
#include "object_registry.hpp"
namespace mousse
{
class surfaceRegistry
:
  public objectRegistry
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    surfaceRegistry(const surfaceRegistry&);
    //- Disallow default bitwise assignment
    void operator=(const surfaceRegistry&);
public:
  //- Runtime type information
  TypeName("surfaceRegistry");
    //- The prefix to local: %surfaces
    static const word prefix;
    //- The default surface name: %default
    static word defaultName;
  // Constructors
    //- Construct for the given objectRegistry and named surface
    surfaceRegistry(const objectRegistry&, const word& surfName = "");
  //- Destructor
  virtual ~surfaceRegistry();
};
}  // namespace mousse
#endif
