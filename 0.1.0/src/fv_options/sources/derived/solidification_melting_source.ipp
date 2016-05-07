// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_matrices.hpp"
#include "fvc_ddt.hpp"


// Private Member Functions 
template<class RhoFieldType>
void mousse::fv::solidificationMeltingSource::apply
(
  const RhoFieldType& rho,
  fvMatrix<scalar>& eqn
)
{
  if (debug) {
    Info << type() << ": applying source to " << eqn.psi().name() << endl;
  }
  const volScalarField Cp{this->Cp()};
  update(Cp);
  dimensionedScalar L{"L", dimEnergy/dimMass, L_};
  // contributions added to rhs of solver equation
  if (eqn.psi().dimensions() == dimTemperature) {
    // isothermal phase change - only include time derivative
    // eqn -= L/Cp*(fvc::ddt(rho, alpha1_) + fvc::div(phi, alpha1_));
    eqn -= L/Cp*(fvc::ddt(rho, alpha1_));
  } else {
    // isothermal phase change - only include time derivative
    // eqn -= L*(fvc::ddt(rho, alpha1_) + fvc::div(phi, alpha1_));
    eqn -= L*(fvc::ddt(rho, alpha1_));
  }
}

