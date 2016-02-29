// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_dependent_model.hpp"
#include "wall_dist.hpp"
// Constructors 
mousse::wallDependentModel::wallDependentModel(const fvMesh& mesh)
:
  mesh_{mesh}
{}
// Destructor 
mousse::wallDependentModel::~wallDependentModel()
{}
// Member Functions 
const mousse::volScalarField& mousse::wallDependentModel::yWall() const
{
  return wallDist::New(mesh_).y();
}
const mousse::volVectorField& mousse::wallDependentModel::nWall() const
{
  return wallDist::New(mesh_).n();
}
