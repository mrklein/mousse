// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coupled_point_patch.hpp"
#include "point_boundary_mesh.hpp"

// Static Data Members
namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(coupledPointPatch, 0);

}

// Constructors
mousse::coupledPointPatch::coupledPointPatch(const pointBoundaryMesh&)
{}

// Destructor
mousse::coupledPointPatch::~coupledPointPatch()
{}
