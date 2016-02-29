#ifndef DYNAMIC_MESH_MOTION_SOLVER_VELOCITY_VELOCITY_MOTION_SOLVER_HPP_
#define DYNAMIC_MESH_MOTION_SOLVER_VELOCITY_VELOCITY_MOTION_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::velocityMotionSolver
// Description
//   Virtual base class for velocity motion solver
//   The boundary displacement is set as a boundary condition
//   on the pointMotionU pointVectorField.
// SourceFiles
//   velocity_motion_solver.cpp
#include "motion_solver.hpp"
#include "point_fields.hpp"
namespace mousse
{
class mapPolyMesh;
class velocityMotionSolver
:
  public motionSolver
{
protected:
  // Protected data
    //- Point motion field
    mutable pointVectorField pointMotionU_;
public:
  //- Runtime type information
  TYPE_NAME("velocityMotionSolver");
  // Constructors
    //- Construct from mesh and dictionary
    velocityMotionSolver
    (
      const polyMesh&,
      const IOdictionary&,
      const word& type
    );
    //- Disallow default bitwise copy construct
    velocityMotionSolver(const velocityMotionSolver&) = delete;
    //- Disallow default bitwise assignment
    velocityMotionSolver& operator=(const velocityMotionSolver&) = delete;
  //- Destructor
  virtual ~velocityMotionSolver();
  // Member Functions
    //- Return reference to the point motion velocity field
    pointVectorField& pointMotionU()
    {
      return pointMotionU_;
    }
    //- Return const reference to the point motion velocity field
    const pointVectorField& pointMotionU() const
    {
      return pointMotionU_;
    }
    //- Update local data for geometry changes
    virtual void movePoints(const pointField&);
    //-  Update local data for topology changes
    virtual void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif
