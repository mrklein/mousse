// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "soot_model.hpp"
// Static Data Members
namespace mousse
{
namespace radiation
{
  DEFINE_TYPE_NAME_AND_DEBUG(sootModel, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(sootModel, dictionary);
}
}
// Constructors 
mousse::radiation::sootModel::sootModel
(
  const dictionary& dict,
  const fvMesh& mesh,
  const word& /*modelType*/
)
:
  dict_{dict},
  mesh_{mesh}
{}
// Destructor   
mousse::radiation::sootModel::~sootModel()
{}
// Member Functions
