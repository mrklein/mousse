// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tabulated_acceleration_source.hpp"
#include "fv_mesh.hpp"
#include "fv_matrices.hpp"
#include "uniform_dimensioned_fields.hpp"
// Private Member Functions 
template<class RhoFieldType>
void mousse::fv::tabulatedAccelerationSource::addSup
(
  const RhoFieldType& rho,
  fvMatrix<vector>& eqn,
  const label fieldi
)
{
  Vector<vector> acceleration(motion_.acceleration());
  // If gravitational force is present combine with the linear acceleration
  if (mesh_.foundObject<uniformDimensionedVectorField>("g"))
  {
    uniformDimensionedVectorField& g =
      const_cast<uniformDimensionedVectorField&>
      (
        mesh_.lookupObject<uniformDimensionedVectorField>("g")
      );
    const uniformDimensionedScalarField& hRef =
      mesh_.lookupObject<uniformDimensionedScalarField>("hRef");
    g = g0_ - dimensionedVector("a", dimAcceleration, acceleration.x());
    dimensionedScalar ghRef
    (
      mag(g.value()) > SMALL
     ? g & (cmptMag(g.value())/mag(g.value()))*hRef
     : dimensionedScalar("ghRef", g.dimensions()*dimLength, 0)
    );
    const_cast<volScalarField&>
    (
      mesh_.lookupObject<volScalarField>("gh")
    ) = (g & mesh_.C()) - ghRef;
    const_cast<surfaceScalarField&>
    (
      mesh_.lookupObject<surfaceScalarField>("ghf")
    ) = (g & mesh_.Cf()) - ghRef;
  }
  // ... otherwise include explicitly in the momentum equation
  else
  {
    eqn -= rho*dimensionedVector("a", dimAcceleration, acceleration.x());
  }
  dimensionedVector Omega
  (
    "Omega",
    dimensionSet(0, 0, -1, 0, 0),
    acceleration.y()
  );
  dimensionedVector dOmegaDT
  (
    "dOmegaDT",
    dimensionSet(0, 0, -2, 0, 0),
    acceleration.z()
  );
  eqn -=
  (
    rho*(2*Omega ^ eqn.psi())         // Coriolis force
   + rho*(Omega ^ (Omega ^ mesh_.C())) // Centrifugal force
   + rho*(dOmegaDT ^ mesh_.C())        // Angular tabulatedAcceleration force
  );
}
