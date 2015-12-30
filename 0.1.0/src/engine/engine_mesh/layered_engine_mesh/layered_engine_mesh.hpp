// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::layeredEngineMesh
// Description
//   mousse::layeredEngineMesh
// SourceFiles
//   layered_engine_mesh.cpp
#ifndef layered_engine_mesh_hpp_
#define layered_engine_mesh_hpp_
#include "engine_mesh.hpp"
#include "dimensioned_scalar.hpp"
namespace mousse
{
class layeredEngineMesh
:
  public engineMesh
{
  // Private data
    dimensionedScalar pistonLayers_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    layeredEngineMesh(const layeredEngineMesh&);
    //- Disallow default bitwise assignment
    void operator=(const layeredEngineMesh&);
public:
  //- Runtime type information
  TypeName("layered");
  // Constructors
    //- Construct from IOobject
    layeredEngineMesh(const IOobject& io);
  //- Destructor
  ~layeredEngineMesh();
  // Member Functions
    // Edit
      void move();
};
}  // namespace mousse
#endif
