// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_to_cell_stencil.hpp"
#include "poly_mesh.hpp"
// Private Member Functions 
// Constructors 
mousse::faceToCellStencil::faceToCellStencil(const polyMesh& mesh)
:
  mesh_(mesh),
  globalNumbering_(mesh_.nFaces())
{}
