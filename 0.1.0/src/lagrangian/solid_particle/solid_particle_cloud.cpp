// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_particle_cloud.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "interpolation_cell_point.hpp"


// Constructors 
mousse::solidParticleCloud::solidParticleCloud
(
  const fvMesh& mesh,
  const word& cloudName,
  bool readFields
)
:
  Cloud<solidParticle>{mesh, cloudName, false},
  mesh_{mesh},
  particleProperties_
  {
    {
      "particleProperties",
      mesh_.time().constant(),
      mesh_,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    }
  },
  rhop_{dimensionedScalar(particleProperties_.lookup("rhop")).value()},
  e_{dimensionedScalar(particleProperties_.lookup("e")).value()},
  mu_{dimensionedScalar(particleProperties_.lookup("mu")).value()}
{
  if (readFields) {
    solidParticle::readFields(*this);
  }
}


// Member Functions 
bool mousse::solidParticleCloud::hasWallImpactDistance() const
{
  return true;
}


void mousse::solidParticleCloud::move(const dimensionedVector& g)
{
  const volScalarField& rho = mesh_.lookupObject<const volScalarField>("rho");
  const volVectorField& U = mesh_.lookupObject<const volVectorField>("U");
  const volScalarField& nu = mesh_.lookupObject<const volScalarField>("nu");
  interpolationCellPoint<scalar> rhoInterp{rho};
  interpolationCellPoint<vector> UInterp{U};
  interpolationCellPoint<scalar> nuInterp{nu};
  solidParticle::trackingData
    td(*this, rhoInterp, UInterp, nuInterp, g.value());
  Cloud<solidParticle>::move(td, mesh_.time().deltaTValue());
}

