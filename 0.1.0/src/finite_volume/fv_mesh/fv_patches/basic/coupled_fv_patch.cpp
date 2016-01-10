// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coupled_fv_patch.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(coupledFvPatch, 0);
}

// Destructor
mousse::coupledFvPatch::~coupledFvPatch()
{}

// Member Functions
mousse::tmp<mousse::vectorField> mousse::coupledFvPatch::delta() const
{
  return Cf() - Cn();
}
