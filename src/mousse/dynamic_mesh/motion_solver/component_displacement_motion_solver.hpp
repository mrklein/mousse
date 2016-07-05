#ifndef DYNAMIC_MESH_MOTION_SOLVER_COMPONENT_DISPLACEMENT_COMPONENT_DISPLACEMENT_MOTION_SOLVER_HPP_
#define DYNAMIC_MESH_MOTION_SOLVER_COMPONENT_DISPLACEMENT_COMPONENT_DISPLACEMENT_MOTION_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::componentDisplacementMotionSolver
// Description
//   Virtual base class for displacement motion solver
//   The boundary displacement is set as a boundary condition
//   on the pointDisplacementX pointScalarField.

#include "motion_solver.hpp"
#include "point_fields.hpp"


namespace mousse {

class mapPolyMesh;


class componentDisplacementMotionSolver
:
  public motionSolver
{
protected:
  // Protected data
    //- The component name to solve for
    word cmptName_;
    //- The component to solve for
    direction cmpt_;
    //- Reference point field for this component
    scalarField points0_;
    //- Point motion field
    mutable pointScalarField pointDisplacement_;
private:
  // Private Member Functions
    //- Return the component corresponding to the given component name
    direction cmpt(const word& cmptName) const;
public:
  //- Runtime type information
  TYPE_NAME("componentDisplacementMotionSolver");
  // Constructors
    //- Construct from polyMesh and dictionary and type
    componentDisplacementMotionSolver
    (
      const polyMesh&,
      const IOdictionary&,
      const word& type
    );
    //- Disallow default bitwise copy construct
    componentDisplacementMotionSolver
    (
      const componentDisplacementMotionSolver&
    ) = delete;
    //- Disallow default bitwise assignment
    componentDisplacementMotionSolver& operator=
    (
      const componentDisplacementMotionSolver&
    ) = delete;
  //- Destructor
  virtual ~componentDisplacementMotionSolver();
  // Member Functions
    //- Return reference to the reference field
    scalarField& points0() { return points0_; }
    //- Return reference to the reference field
    const scalarField& points0() const { return points0_; }
    //- Update local data for geometry changes
    virtual void movePoints(const pointField&);
    //-  Update local data for topology changes
    virtual void updateMesh(const mapPolyMesh&);
};

}  // namespace mousse

#endif

