// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "displacement_motion_solver.hpp"
#include "map_poly_mesh.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(displacementMotionSolver, 0);

}


// Protected Data Members
mousse::IOobject mousse::displacementMotionSolver::points0IO
(
  const polyMesh& mesh
) const
{
  const word instance =
    time().findInstance(mesh.meshDir(), "points0", IOobject::READ_IF_PRESENT);
  if (instance != time().constant()) {
    // points0 written to a time folder
    return
      IOobject
      {
        "points0",
        instance,
        polyMesh::meshSubDir,
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      };
  } else {
    // check that points0 are actually in constant directory
    IOobject io
    {
      "points0",
      instance,
      polyMesh::meshSubDir,
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    };
    if (io.headerOk()) {
      return io;
    } else {
      // copy of original mesh points
      return
        IOobject
        {
          "points",
          instance,
          polyMesh::meshSubDir,
          mesh,
          IOobject::MUST_READ,
          IOobject::NO_WRITE,
          false
        };
    }
  }
}


// Constructors 
mousse::displacementMotionSolver::displacementMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict,
  const word& type
)
:
  motionSolver{mesh, dict, type},
  pointDisplacement_
  {
    {
      "pointDisplacement",
      time().timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    pointMesh::New(mesh)
  },
  points0_{pointIOField(points0IO(mesh))}
{
  if (points0_.size() != mesh.nPoints()) {
    FATAL_ERROR_IN
    (
      "displacementMotionSolver::"
      "displacementMotionSolver\n"
      "(\n"
      "  const polyMesh&,\n"
      "  const IOdictionary&,\n"
      "  const word&\n"
      ")"
    )
    << "Number of points in mesh " << mesh.nPoints()
    << " differs from number of points " << points0_.size()
    << " read from file "
    <<
    IOobject
    {
      "points",
      time().constant(),
      polyMesh::meshSubDir,
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }.filePath()
    << exit(FatalError);
  }
}


// Destructor 
mousse::displacementMotionSolver::~displacementMotionSolver()
{}


// Member Functions 
void mousse::displacementMotionSolver::movePoints(const pointField&)
{
  // No local data to update
}


void mousse::displacementMotionSolver::updateMesh(const mapPolyMesh& mpm)
{
  // pointMesh already updates pointFields
  motionSolver::updateMesh(mpm);
  // Map points0_. Bit special since we somehow have to come up with
  // a sensible points0 position for introduced points.
  // Find out scaling between points0 and current points
  // Get the new points either from the map or the mesh
  const pointField& points =
    mpm.hasMotionPoints() ? mpm.preMotionPoints() : mesh().points();
  // Note: boundBox does reduce
  const vector span0 = boundBox{points0_}.span();
  const vector span = boundBox{points}.span();
  vector scaleFactors{cmptDivide(span0, span)};
  pointField newPoints0{mpm.pointMap().size()};
  FOR_ALL(newPoints0, pointI) {
    label oldPointI = mpm.pointMap()[pointI];
    if (oldPointI >= 0) {
      label masterPointI = mpm.reversePointMap()[oldPointI];
      if (masterPointI == pointI) {
        newPoints0[pointI] = points0_[oldPointI];
      } else {
        // New point - assume motion is scaling
        newPoints0[pointI] = points0_[oldPointI]
          + cmptMultiply(scaleFactors, points[pointI] - points[masterPointI]);
      }
    } else {
      FATAL_ERROR_IN
      (
        "displacementMotionSolver::updateMesh"
        "(const mapPolyMesh&)"
      )
      << "Cannot determine co-ordinates of introduced vertices."
      << " New vertex " << pointI << " at co-ordinate "
      << points[pointI] << exit(FatalError);
    }
  }
  twoDCorrectPoints(newPoints0);
  points0_.transfer(newPoints0);
  // points0 changed - set to write and check-in to database
  points0_.rename("points0");
  points0_.writeOpt() = IOobject::AUTO_WRITE;
  points0_.instance() = time().timeName();
  points0_.checkIn();
}

