// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "topo_changer_fv_mesh.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(topoChangerFvMesh, 0);
}
// Constructors 
mousse::topoChangerFvMesh::topoChangerFvMesh(const IOobject& io)
:
  dynamicFvMesh(io),
  topoChanger_(*this)
{}
// Destructor 
mousse::topoChangerFvMesh::~topoChangerFvMesh()
{}
