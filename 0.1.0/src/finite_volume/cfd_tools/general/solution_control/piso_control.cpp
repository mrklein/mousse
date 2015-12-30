// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "piso_control.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(pisoControl, 0);
}
// Constructors 
mousse::pisoControl::pisoControl(fvMesh& mesh, const word& dictName)
:
  pimpleControl(mesh, dictName)
{}
// Destructor 
mousse::pisoControl::~pisoControl()
{}
