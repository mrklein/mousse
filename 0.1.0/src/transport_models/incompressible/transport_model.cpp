// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transport_model.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(transportModel, 0);
}
// Constructors 
mousse::transportModel::transportModel()
{}
// Destructor 
mousse::transportModel::~transportModel()
{}
// Member Functions 
bool mousse::transportModel::read()
{
  return true;
}
