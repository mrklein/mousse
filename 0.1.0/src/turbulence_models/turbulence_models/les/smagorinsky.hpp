#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_SMAGORINSKY_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_SMAGORINSKY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::Smagorinsky
// Group
//   grpLESTurbulence
// Description
//   The Smagorinsky SGS model.
//   Reference:
//   \verbatim
//     Smagorinsky, J. (1963).
//     General circulation experiments with the primitive equations: I.
//     The basic experiment*.
//     Monthly weather review, 91(3), 99-164.
//   \endverbatim
//   The form of the Smagorinsky model implemented is obtained from the
//   k-equation model assuming local equilibrium which provides estimates of both
//   k and epsilon separate from the sub-grid scale viscosity:
//   \verbatim
//     B = 2/3*k*I - 2*nuSgs*dev(D)
//   where
//     D = symm(grad(U));
//     k from D:B + Ce*k^3/2/delta = 0
//     nuSgs = Ck*sqrt(k)*delta
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     SmagorinskyCoeffs
//     {
//       Ck                  0.094;
//       Ce                  1.048;
//     }
//   \endverbatim
// SeeAlso
//   mousse::LESModels::kEqn

#include "les_model.hpp"
#include "les_eddy_viscosity.hpp"


namespace mousse {
namespace LESModels {

template<class BasicTurbulenceModel>
class Smagorinsky
:
  public LESeddyViscosity<BasicTurbulenceModel>
{
protected:
  // Protected data
    dimensionedScalar Ck_;
  // Protected Member Functions
    //- Return SGS kinetic energy
    //  calculated from the given velocity gradient
    tmp<volScalarField> k(const tmp<volTensorField>& gradU) const;
    //- Update the SGS eddy viscosity
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("Smagorinsky");
  // Constructors
    //- Construct from components
    Smagorinsky
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName,
      const word& type = typeName
    );
    // Disallow default bitwise copy construct and assignment
    Smagorinsky(const Smagorinsky&) = delete;
    Smagorinsky& operator=(const Smagorinsky&) = delete;
  //- Destructor
  virtual ~Smagorinsky()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const
    {
      return k(fvc::grad(this->U_));
    }
    //- Return sub-grid disipation rate
    virtual tmp<volScalarField> epsilon() const;
    //- Correct Eddy-Viscosity and related properties
    virtual void correct();
};

}  // namespace LESModels
}  // namespace mousse

#include "smagorinsky.ipp"

#endif

