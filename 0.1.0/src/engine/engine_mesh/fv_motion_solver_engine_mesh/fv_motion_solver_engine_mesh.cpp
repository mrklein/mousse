// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_motion_solver_engine_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fvc_mesh_phi.hpp"
#include "surface_interpolate.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(fvMotionSolverEngineMesh, 0);
  addToRunTimeSelectionTable(engineMesh, fvMotionSolverEngineMesh, IOobject);
}
// Constructors 
mousse::fvMotionSolverEngineMesh::fvMotionSolverEngineMesh(const IOobject& io)
:
  engineMesh(io),
  pistonLayers_("pistonLayers", dimLength, 0.0),
  motionSolver_
  (
    *this,
    engineDB_.engineDict()
  )
{
  engineDB_.engineDict().readIfPresent("pistonLayers", pistonLayers_);
}
// Destructor 
mousse::fvMotionSolverEngineMesh::~fvMotionSolverEngineMesh()
{}
// Member Functions 
void mousse::fvMotionSolverEngineMesh::move()
{
  scalar deltaZ = engineDB_.pistonDisplacement().value();
  Info<< "deltaZ = " << deltaZ << endl;
  // Position of the top of the static mesh layers above the piston
  scalar pistonPlusLayers = pistonPosition_.value() + pistonLayers_.value();
  scalar pistonSpeed = deltaZ/engineDB_.deltaTValue();
  motionSolver_.pointMotionU().boundaryField()[pistonIndex_] == pistonSpeed;
  {
    scalarField linerPoints
    (
      boundary()[linerIndex_].patch().localPoints().component(vector::Z)
    );
    motionSolver_.pointMotionU().boundaryField()[linerIndex_] ==
      pistonSpeed*pos(deckHeight_.value() - linerPoints)
     *(deckHeight_.value() - linerPoints)
     /(deckHeight_.value() - pistonPlusLayers);
  }
  motionSolver_.solve();
  if (engineDB_.foundObject<surfaceScalarField>("phi"))
  {
    surfaceScalarField& phi =
      const_cast<surfaceScalarField&>
      (engineDB_.lookupObject<surfaceScalarField>("phi"));
    const volScalarField& rho =
      engineDB_.lookupObject<volScalarField>("rho");
    const volVectorField& U =
      engineDB_.lookupObject<volVectorField>("U");
    bool absolutePhi = false;
    if (moving())
    {
      phi += fvc::interpolate(rho)*fvc::meshPhi(rho, U);
      absolutePhi = true;
    }
    movePoints(motionSolver_.curPoints());
    if (absolutePhi)
    {
      phi -= fvc::interpolate(rho)*fvc::meshPhi(rho, U);
    }
  }
  else
  {
    movePoints(motionSolver_.curPoints());
  }
  pistonPosition_.value() += deltaZ;
  Info<< "clearance: " << deckHeight_.value() - pistonPosition_.value() << nl
    << "Piston speed = " << pistonSpeed << " m/s" << endl;
}
