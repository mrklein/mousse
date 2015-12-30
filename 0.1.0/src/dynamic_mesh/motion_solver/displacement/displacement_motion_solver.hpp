// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::displacementMotionSolver
// Description
//   Virtual base class for displacement motion solver
//   The boundary displacement is set as a boundary condition
//   on the pointDisplacement pointVectorField.
// SourceFiles
//   displacement_motion_solver.cpp
#ifndef displacement_motion_solver_hpp_
#define displacement_motion_solver_hpp_
#include "motion_solver.hpp"
#include "point_fields.hpp"
#include "point_io_field.hpp"
namespace mousse
{
class mapPolyMesh;
class displacementMotionSolver
:
  public motionSolver
{
protected:
  // Protected data
    //- Point motion field
    mutable pointVectorField pointDisplacement_;
    //- Starting points
    pointIOField points0_;
  // Protected Member Functions
    //- Return IO object for points0
    IOobject points0IO(const polyMesh& mesh) const;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    displacementMotionSolver(const displacementMotionSolver&);
    //- Disallow default bitwise assignment
    void operator=(const displacementMotionSolver&);
public:
  //- Runtime type information
  TypeName("displacementMotionSolver");
  // Constructors
    //- Construct from mesh and dictionary
    displacementMotionSolver
    (
      const polyMesh&,
      const IOdictionary&,
      const word& type
    );
  //- Destructor
  virtual ~displacementMotionSolver();
  // Member Functions
    //- Return reference to the reference field
    pointField& points0()
    {
      return points0_;
    }
    //- Return reference to the reference field
    const pointField& points0() const
    {
      return points0_;
    }
    //- Return reference to the point motion displacement field
    pointVectorField& pointDisplacement()
    {
      return pointDisplacement_;
    }
    //- Return const reference to the point motion displacement field
    const pointVectorField& pointDisplacement() const
    {
      return pointDisplacement_;
    }
    //- Update local data for geometry changes
    virtual void movePoints(const pointField&);
    //-  Update local data for topology changes
    virtual void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif
