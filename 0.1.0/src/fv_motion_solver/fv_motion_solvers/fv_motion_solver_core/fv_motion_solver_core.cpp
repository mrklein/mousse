// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_motion_solver_core.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(fvMotionSolverCore, 0);
}
// Constructors 
mousse::fvMotionSolverCore::fvMotionSolverCore(const polyMesh& mesh)
:
  fvMesh_(refCast<const fvMesh>(mesh))
{}
