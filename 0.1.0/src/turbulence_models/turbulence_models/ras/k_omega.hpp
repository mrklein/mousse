// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::kOmega
// Group
//   grpRASTurbulence
// Description
//   Standard high Reynolds-number k-omega turbulence model for
//   incompressible and compressible flows.
//   References:
//   \verbatim
//     Wilcox, D. C. (1998).
//     Turbulence modeling for CFD
//     (Vol. 2, pp. 103-217). La Canada, CA: DCW industries.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     kOmegaCoeffs
//     {
//       Cmu         0.09;  // Equivalent to betaStar
//       alpha       0.52;
//       beta        0.072;
//       alphak      0.5;
//       alphaOmega  0.5;
//     }
//   \endverbatim
// SourceFiles
//   k_omega.cpp
#ifndef k_omega_hpp_
#define k_omega_hpp_
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class kOmega
:
  public eddyViscosity<RASModel<BasicTurbulenceModel> >
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmu_;
      dimensionedScalar beta_;
      dimensionedScalar gamma_;
      dimensionedScalar alphaK_;
      dimensionedScalar alphaOmega_;
    // Fields
      volScalarField k_;
      volScalarField omega_;
  // Protected Member Functions
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("kOmega");
  // Constructors
    //- Construct from components
    kOmega
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
  //- Destructor
  virtual ~kOmega()
  {}
  // Member Functions
    //- Read RASProperties dictionary
    virtual bool read();
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          "DkEff",
          alphaK_*this->nut_ + this->nu()
        )
      );
    }
    //- Return the effective diffusivity for omega
    tmp<volScalarField> DomegaEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          "DomegaEff",
          alphaOmega_*this->nut_ + this->nu()
        )
      );
    }
    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const
    {
      return k_;
    }
    //- Return the turbulence specific dissipation rate
    virtual tmp<volScalarField> omega() const
    {
      return omega_;
    }
    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> epsilon() const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          IOobject
          (
            "epsilon",
            this->mesh_.time().timeName(),
            this->mesh_
          ),
          Cmu_*k_*omega_,
          omega_.boundaryField().types()
        )
      );
    }
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};
}  // namespace RASModels
}  // namespace mousse
#ifdef NoRepository
#   include "k_omega.cpp"
#endif
#endif
