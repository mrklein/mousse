// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "six_dof_rigid_body_motion_solver.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "poly_mesh.hpp"
#include "point_patch_dist.hpp"
#include "point_constraints.hpp"
#include "uniform_dimensioned_fields.hpp"
#include "forces.hpp"
#include "mathematical_constants.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(sixDoFRigidBodyMotionSolver, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    motionSolver,
    sixDoFRigidBodyMotionSolver,
    dictionary
  );
}
// Constructors 
mousse::sixDoFRigidBodyMotionSolver::sixDoFRigidBodyMotionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict
)
:
  displacementMotionSolver(mesh, dict, typeName),
  motion_
  (
    coeffDict(),
    IOobject
    (
      "sixDoFRigidBodyMotionState",
      mesh.time().timeName(),
      "uniform",
      mesh
    ).headerOk()
   ? IOdictionary
    (
      IOobject
      (
        "sixDoFRigidBodyMotionState",
        mesh.time().timeName(),
        "uniform",
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE,
        false
      )
    )
   : coeffDict()
  ),
  patches_(wordReList(coeffDict().lookup("patches"))),
  patchSet_(mesh.boundaryMesh().patchSet(patches_)),
  di_(readScalar(coeffDict().lookup("innerDistance"))),
  do_(readScalar(coeffDict().lookup("outerDistance"))),
  test_(coeffDict().lookupOrDefault<Switch>("test", false)),
  rhoInf_(1.0),
  rhoName_(coeffDict().lookupOrDefault<word>("rhoName", "rho")),
  scale_
  (
    IOobject
    (
      "motionScale",
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    ),
    pointMesh::New(mesh),
    dimensionedScalar("zero", dimless, 0.0)
  ),
  curTimeIndex_(-1)
{
  if (rhoName_ == "rhoInf")
  {
    rhoInf_ = readScalar(coeffDict().lookup("rhoInf"));
  }
  // Calculate scaling factor everywhere
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {
    const pointMesh& pMesh = pointMesh::New(mesh);
    pointPatchDist pDist(pMesh, patchSet_, points0());
    // Scaling: 1 up to di then linear down to 0 at do away from patches
    scale_.internalField() =
      min
      (
        max
        (
          (do_ - pDist.internalField())/(do_ - di_),
          scalar(0)
        ),
        scalar(1)
      );
    // Convert the scale function to a cosine
    scale_.internalField() =
      min
      (
        max
        (
          0.5
         - 0.5
         *cos(scale_.internalField()
         *mousse::constant::mathematical::pi),
          scalar(0)
        ),
        scalar(1)
      );
    pointConstraints::New(pMesh).constrain(scale_);
    scale_.write();
  }
}
// Destructor 
mousse::sixDoFRigidBodyMotionSolver::~sixDoFRigidBodyMotionSolver()
{}
// Member Functions 
mousse::tmp<mousse::pointField>
mousse::sixDoFRigidBodyMotionSolver::curPoints() const
{
  return points0() + pointDisplacement_.internalField();
}
void mousse::sixDoFRigidBodyMotionSolver::solve()
{
  const Time& t = mesh().time();
  if (mesh().nPoints() != points0().size())
  {
    FATAL_ERROR_IN
    (
      "sixDoFRigidBodyMotionSolver::curPoints() const"
    )   << "The number of points in the mesh seems to have changed." << endl
      << "In constant/polyMesh there are " << points0().size()
      << " points; in the current mesh there are " << mesh().nPoints()
      << " points." << exit(FatalError);
  }
  // Store the motion state at the beginning of the time-stepbool
  bool firstIter = false;
  if (curTimeIndex_ != this->db().time().timeIndex())
  {
    motion_.newTime();
    curTimeIndex_ = this->db().time().timeIndex();
    firstIter = true;
  }
  dimensionedVector g("g", dimAcceleration, vector::zero);
  if (db().foundObject<uniformDimensionedVectorField>("g"))
  {
    g = db().lookupObject<uniformDimensionedVectorField>("g");
  }
  else if (coeffDict().found("g"))
  {
    coeffDict().lookup("g") >> g;
  }
  // scalar ramp = min(max((this->db().time().value() - 5)/10, 0), 1);
  scalar ramp = 1.0;
  if (test_)
  {
    motion_.update
    (
      firstIter,
      ramp*(motion_.mass()*g.value()),
      ramp*(motion_.mass()*(motion_.momentArm() ^ g.value())),
      t.deltaTValue(),
      t.deltaT0Value()
    );
  }
  else
  {
    dictionary forcesDict;
    forcesDict.add("type", forces::typeName);
    forcesDict.add("patches", patches_);
    forcesDict.add("rhoInf", rhoInf_);
    forcesDict.add("rhoName", rhoName_);
    forcesDict.add("CofR", motion_.centreOfRotation());
    forces f("forces", db(), forcesDict);
    f.calcForcesMoment();
    motion_.update
    (
      firstIter,
      ramp*(f.forceEff() + motion_.mass()*g.value()),
      ramp
     *(
       f.momentEff()
      + motion_.mass()*(motion_.momentArm() ^ g.value())
      ),
      t.deltaTValue(),
      t.deltaT0Value()
    );
  }
  // Update the displacements
  pointDisplacement_.internalField() =
    motion_.transform(points0(), scale_) - points0();
  // Displacement has changed. Update boundary conditions
  pointConstraints::New
  (
    pointDisplacement_.mesh()
  ).constrainDisplacement(pointDisplacement_);
}
bool mousse::sixDoFRigidBodyMotionSolver::writeObject
(
  IOstream::streamFormat /*fmt*/,
  IOstream::versionNumber /*ver*/,
  IOstream::compressionType /*cmp*/
) const
{
  IOdictionary dict
  {
    // IOobject
    {
      "sixDoFRigidBodyMotionState",
      mesh().time().timeName(),
      "uniform",
      mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  motion_.state().write(dict);
  return dict.regIOobject::write();
}
bool mousse::sixDoFRigidBodyMotionSolver::read()
{
  if (displacementMotionSolver::read())
  {
    motion_.read(coeffDict());
    return true;
  }
  else
  {
    return false;
  }
}
