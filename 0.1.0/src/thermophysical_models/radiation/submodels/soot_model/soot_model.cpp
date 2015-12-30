// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "soot_model.hpp"
// Static Data Members
namespace mousse
{
namespace radiation
{
  defineTypeNameAndDebug(sootModel, 0);
  defineRunTimeSelectionTable(sootModel, dictionary);
}
}
// Constructors 
mousse::radiation::sootModel::sootModel
(
  const dictionary& dict,
  const fvMesh& mesh,
  const word& modelType
)
:
  dict_(dict),
  mesh_(mesh)
{}
// Destructor   
mousse::radiation::sootModel::~sootModel()
{}
// Member Functions
