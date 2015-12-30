// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::incompressible::RASModels::ShihQuadraticKE
// Group
//   grpIcoRASTurbulence
// Description
//   Shih's quadratic algebraic Reynolds stress k-epsilon turbulence model for
//   incompressible flows
//   This turbulence model is described in:
//   \verbatim
//     Shih, T. H., Zhu, J., & Lumley, J. L. (1993).
//     A realizable Reynolds stress algebraic equation model.
//     NASA technical memorandum 105993.
//   \endverbatim
//   Implemented according to the specification in:
//   <a href=
//   "http://personalpages.manchester.ac.uk/staff/david.d.apsley/specturb.pdf"
//   >Apsley: Turbulence Models 2002</a>
// SourceFiles
//   shih_quadratic_ke.cpp
#ifndef shih_quadratic_ke_hpp_
#define shih_quadratic_ke_hpp_
#include "turbulent_transport_model.hpp"
#include "nonlinear_eddy_viscosity.hpp"
namespace mousse
{
namespace incompressible
{
namespace RASModels
{
class ShihQuadraticKE
:
  public nonlinearEddyViscosity<incompressible::RASModel>
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Ceps1_;
      dimensionedScalar Ceps2_;
      dimensionedScalar sigmak_;
      dimensionedScalar sigmaEps_;
      dimensionedScalar Cmu1_;
      dimensionedScalar Cmu2_;
      dimensionedScalar Cbeta_;
      dimensionedScalar Cbeta1_;
      dimensionedScalar Cbeta2_;
      dimensionedScalar Cbeta3_;
    // Fields
      volScalarField k_;
      volScalarField epsilon_;
  // Protected Member Functions
    virtual void correctNut();
    virtual void correctNonlinearStress(const volTensorField& gradU);
public:
  //- Runtime type information
  TypeName("ShihQuadraticKE");
  // Constructors
    //- Construct from components
    ShihQuadraticKE
    (
      const geometricOneField& alpha,
      const geometricOneField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName,
      const word& type = typeName
    );
  //- Destructor
  virtual ~ShihQuadraticKE()
  {}
  // Member Functions
    //- Read RASProperties dictionary
    virtual bool read();
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField("DkEff", nut_/sigmak_ + nu())
      );
    }
    //- Return the effective diffusivity for epsilon
    tmp<volScalarField> DepsilonEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField("DepsilonEff", nut_/sigmaEps_ + nu())
      );
    }
    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const
    {
      return k_;
    }
    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> epsilon() const
    {
      return epsilon_;
    }
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};
}  // namespace RASModels
}  // namespace incompressible
}  // namespace mousse
#endif
