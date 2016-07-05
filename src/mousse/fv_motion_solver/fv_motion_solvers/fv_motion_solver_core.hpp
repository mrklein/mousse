#ifndef FV_MOTION_SOLVER_FV_MOTION_SOLVERS_FV_MOTION_SOLVER_CORE_FV_MOTION_SOLVER_CORE_HPP_
#define FV_MOTION_SOLVER_FV_MOTION_SOLVERS_FV_MOTION_SOLVER_CORE_FV_MOTION_SOLVER_CORE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvMotionSolverCore
// Description
//   Base class for fvMesh based motionSolvers.

#include "fv_mesh.hpp"


namespace mousse {

class fvMotionSolverCore
{
protected:
  // Protected data
    //- The fvMesh to be moved
    const fvMesh& fvMesh_;
  // Protected Member Functions
    //- Create the corresponding patch types for cellMotion from those
    //  of the given pointMotion
    template<class Type>
    wordList cellMotionBoundaryTypes
    (
      const typename GeometricField<Type, pointPatchField, pointMesh>::
      GeometricBoundaryField& pmUbf
    ) const;
public:
  //- Runtime type information
  CLASS_NAME("displacementMotionSolver");
  // Constructors
    //- Construct from polyMesh
    fvMotionSolverCore(const polyMesh&);
  // Member Functions
    //- Return reference to the fvMesh to be moved
    const fvMesh& mesh() const { return fvMesh_; }
};

}  // namespace mousse

#include "fv_motion_solver_core.ipp"

#endif
