// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dim_field_decomposer.hpp"


// Constructors 
mousse::dimFieldDecomposer::dimFieldDecomposer
(
  const fvMesh& /*completeMesh*/,
  const fvMesh& procMesh,
  const labelList& /*faceAddressing*/,
  const labelList& cellAddressing
)
:
  // completeMesh_{completeMesh},
  procMesh_{procMesh},
  // faceAddressing_{faceAddressing},
  cellAddressing_{cellAddressing}
{}


// Destructor 
mousse::dimFieldDecomposer::~dimFieldDecomposer()
{}

