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
#ifndef velocity_motion_solver_hpp_
#define velocity_motion_solver_hpp_
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
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    velocityMotionSolver
    (
      const velocityMotionSolver&
    );
    //- Disallow default bitwise assignment
    void operator=(const velocityMotionSolver&);
public:
  //- Runtime type information
  TypeName("velocityMotionSolver");
  // Constructors
    //- Construct from mesh and dictionary
    velocityMotionSolver
    (
      const polyMesh&,
      const IOdictionary&,
      const word& type
    );
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
