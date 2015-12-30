// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "scatter_model.hpp"
// Static Data Members
namespace mousse
{
  namespace radiation
  {
    defineTypeNameAndDebug(scatterModel, 0);
    defineRunTimeSelectionTable(scatterModel, dictionary);
  }
}
// Constructors 
mousse::radiation::scatterModel::scatterModel
(
  const dictionary&,
  const fvMesh& mesh
)
:
  mesh_(mesh)
{}
// Destructor   
mousse::radiation::scatterModel::~scatterModel()
{}
