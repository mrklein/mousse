// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::componentVelocityMotionSolver
// Description
//   Virtual base class for velocity motion solver
//   The boundary displacement is set as a boundary condition
//   on the pointMotionUX pointScalarField.
// SourceFiles
//   component_velocity_motion_solver.cpp
#ifndef component_velocity_motion_solver_hpp_
#define component_velocity_motion_solver_hpp_
#include "motion_solver.hpp"
#include "point_fields.hpp"
namespace mousse
{
class mapPolyMesh;
class componentVelocityMotionSolver
:
  public motionSolver
{
protected:
  // Protected data
    //- The component name to solve for
    word cmptName_;
    //- The component to solve for
    direction cmpt_;
    //- Point motion field
    mutable pointScalarField pointMotionU_;
private:
  // Private Member Functions
    //- Return the component corresponding to the given component name
    direction cmpt(const word& cmptName) const;
    //- Disallow default bitwise copy construct
    componentVelocityMotionSolver
    (
      const componentVelocityMotionSolver&
    );
    //- Disallow default bitwise assignment
    void operator=(const componentVelocityMotionSolver&);
public:
  //- Runtime type information
  TypeName("componentVelocityMotionSolver");
  // Constructors
    //- Construct from mesh and dictionary
    componentVelocityMotionSolver
    (
      const polyMesh&,
      const IOdictionary&,
      const word& type
    );
  //- Destructor
  virtual ~componentVelocityMotionSolver();
  // Member Functions
    //- Non-const access to the pointMotionU in order to allow changes
    //  to the boundary motion
    pointScalarField& pointMotionU()
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
