// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::kEqn
// Group
//   grpLESTurbulence
// Description
//   One equation eddy-viscosity model
//   Eddy viscosity SGS model using a modeled balance equation to simulate the
//   behaviour of k.
//   Reference:
//   \verbatim
//     Yoshizawa, A. (1986).
//     Statistical theory for compressible turbulent shear flows,
//     with the application to subgrid modeling.
//     Physics of Fluids (1958-1988), 29(7), 2152-2164.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     kEqnCoeffs
//     {
//       Ck                  0.094;
//       Ce                  1.048;
//     }
//   \endverbatim
// SourceFiles
//   k_eqn.cpp
#ifndef k_eqn_hpp_
#define k_eqn_hpp_
#include "les_eddy_viscosity.hpp"
namespace mousse
{
namespace LESModels
{
template<class BasicTurbulenceModel>
class kEqn
:
  public LESeddyViscosity<BasicTurbulenceModel>
{
protected:
  // Protected data
    // Fields
      volScalarField k_;
    // Model constants
      dimensionedScalar Ck_;
  // Protected Member Functions
    virtual void correctNut();
    virtual tmp<fvScalarMatrix> kSource() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("kEqn");
  // Constructors
    //- Constructor from components
    kEqn
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
    kEqn(const kEqn&) = delete;
    kEqn& operator=(const kEqn&) = delete;
  //- Destructor
  virtual ~kEqn()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const
    {
      return k_;
    }
    //- Return sub-grid disipation rate
    virtual tmp<volScalarField> epsilon() const;
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField("DkEff", this->nut_ + this->nu())
      );
    }
    //- Correct eddy-Viscosity and related properties
    virtual void correct();
};
}  // namespace LESModels
}  // namespace mousse
#ifdef NoRepository
#   include "k_eqn.cpp"
#endif
#endif
