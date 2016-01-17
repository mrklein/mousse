// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "layered_engine_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fvc_mesh_phi.hpp"
#include "surface_interpolate.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(layeredEngineMesh, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(engineMesh, layeredEngineMesh, IOobject);
}
// Constructors 
mousse::layeredEngineMesh::layeredEngineMesh(const IOobject& io)
:
  engineMesh(io),
  pistonLayers_("pistonLayers", dimLength, 0.0)
{
  engineDB_.engineDict().readIfPresent("pistonLayers", pistonLayers_);
}
// Destructor 
mousse::layeredEngineMesh::~layeredEngineMesh()
{}
// Member Functions 
void mousse::layeredEngineMesh::move()
{
  scalar deltaZ = engineDB_.pistonDisplacement().value();
  Info<< "deltaZ = " << deltaZ << endl;
  // Position of the top of the static mesh layers above the piston
  scalar pistonPlusLayers = pistonPosition_.value() + pistonLayers_.value();
  pointField newPoints(points());
  FOR_ALL(newPoints, pointi)
  {
    point& p = newPoints[pointi];
    if (p.z() < pistonPlusLayers)           // In piston bowl
    {
      p.z() += deltaZ;
    }
    else if (p.z() < deckHeight_.value())   // In liner region
    {
      p.z() +=
        deltaZ
       *(deckHeight_.value() - p.z())
       /(deckHeight_.value() - pistonPlusLayers);
    }
  }
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
    movePoints(newPoints);
    if (absolutePhi)
    {
      phi -= fvc::interpolate(rho)*fvc::meshPhi(rho, U);
    }
  }
  else
  {
    movePoints(newPoints);
  }
  pistonPosition_.value() += deltaZ;
  scalar pistonSpeed = deltaZ/engineDB_.deltaTValue();
  Info<< "clearance: " << deckHeight_.value() - pistonPosition_.value() << nl
    << "Piston speed = " << pistonSpeed << " m/s" << endl;
}
