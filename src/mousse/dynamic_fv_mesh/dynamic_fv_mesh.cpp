// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_fv_mesh.hpp"


// Static Data Members

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(dynamicFvMesh, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(dynamicFvMesh, IOobject);

}


// Constructors 
mousse::dynamicFvMesh::dynamicFvMesh(const IOobject& io)
:
  fvMesh{io}
{}


mousse::dynamicFvMesh::dynamicFvMesh
(
  const IOobject& io,
  const Xfer<pointField>& points,
  const Xfer<faceList>& faces,
  const Xfer<labelList>& allOwner,
  const Xfer<labelList>& allNeighbour,
  const bool syncPar
)
:
  fvMesh{io, points, faces, allOwner, allNeighbour, syncPar}
{}


mousse::dynamicFvMesh::dynamicFvMesh
(
  const IOobject& io,
  const Xfer<pointField>& points,
  const Xfer<faceList>& faces,
  const Xfer<cellList>& cells,
  const bool syncPar
)
:
  fvMesh{io, points, faces, cells, syncPar}
{}


// Destructor 
mousse::dynamicFvMesh::~dynamicFvMesh()
{}

