#ifndef TOPO_CHANGER_FV_MESH_TOPO_CHANGER_FV_MESH_TOPO_CHANGER_FV_MESH_HPP_
#define TOPO_CHANGER_FV_MESH_TOPO_CHANGER_FV_MESH_TOPO_CHANGER_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::topoChangerFvMesh
// Description
//   Abstract base class for a topology changing fvMesh
// SourceFiles
//   topo_changer_fv_mesh.cpp
//   new_topo_fv_mesh.cpp
#include "dynamic_fv_mesh.hpp"
#include "poly_topo_changer.hpp"
namespace mousse
{
class topoChangerFvMesh
:
  public dynamicFvMesh
{

protected:
  // Protected Data
    polyTopoChanger topoChanger_;
public:
  //- Runtime type information
  TYPE_NAME("topoChangerFvMesh");
  // Constructors
    //- Construct from objectRegistry, and read/write options
    explicit topoChangerFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    topoChangerFvMesh(const topoChangerFvMesh&) = delete;
    //- Disallow default bitwise assignment
    topoChangerFvMesh& operator=(const topoChangerFvMesh&) = delete;
  //- Destructor
  virtual ~topoChangerFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update() = 0;
};
}  // namespace mousse
#endif
