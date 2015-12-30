// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::displacementInterpolationMotionSolver
// Description
//   Mesh motion solver for an fvMesh.
//   Scales inbetween motion prescribed on faceZones. Works out per point
//   the distance between the bounding face zones (in all three directions)
//   at the start and then every time step
//   - moves the faceZones based on tables
//   - interpolates the displacement of all points based on the
//    faceZone motion.
//   Tables are in the \a constant/tables directory.
// SourceFiles
//   displacement_interpolation_motion_solver.cpp
#ifndef displacement_interpolation_motion_solver_hpp_
#define displacement_interpolation_motion_solver_hpp_
#include "displacement_motion_solver.hpp"
namespace mousse
{
class displacementInterpolationMotionSolver
:
  public displacementMotionSolver
{
  // Private data
    // Face zone information (note: could pack these to only contain
    // used zones)
      //- Interpolation table. From faceZone to times.
      List<scalarField> times_;
      //- Interpolation table. From faceZone to displacements.
      List<vectorField> displacements_;
    // Range information.
      //- Per direction, per range the index of the lower
      //  faceZone
      FixedList<labelList, 3> rangeToZone_;
      //- Per direction, per range the points that are in it
      FixedList<labelListList, 3> rangeToPoints_;
      //- Per direction, per range the weight of the points relative
      //  to this and the next range.
      FixedList<List<scalarField>, 3> rangeToWeights_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    displacementInterpolationMotionSolver
    (
      const displacementInterpolationMotionSolver&
    );
    //- Disallow default bitwise assignment
    void operator=(const displacementInterpolationMotionSolver&);
public:
  //- Runtime type information
  TypeName("displacementInterpolation");
  // Constructors
    //- Construct from polyMesh and IOdictionary
    displacementInterpolationMotionSolver
    (
      const polyMesh&,
      const IOdictionary& dict
    );
  //- Destructor
  ~displacementInterpolationMotionSolver();
  // Member Functions
    //- Return point location obtained from the current motion field
    virtual tmp<pointField> curPoints() const;
    //- Solve for motion
    virtual void solve()
    {}
};
}  // namespace mousse
#endif
