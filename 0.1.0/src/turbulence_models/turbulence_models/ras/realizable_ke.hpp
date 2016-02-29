#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_REALIZABLE_KE_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_REALIZABLE_KE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::realizableKE
// Group
//   grpRASTurbulence
// Description
//   Realizable k-epsilon turbulence model for incompressible and compressible
//   flows.
//   References:
//   \verbatim
//     Shih, T. H., Liou, W. W., Shabbir, A., Yang, Z., & Zhu, J. (1994).
//     A new k-epsilon eddy viscosity model for high Reynolds number
//     turbulent flows: Model development and validation.
//     NASA STI/Recon Technical Report N, 95, 11442.
//     Shih, T. H., Liou, W. W., Shabbir, A., Yang, Z., & Zhu, J. (1995).
//     A New k-epsilon Eddy Viscosity Model for High Reynolds Number
//     Turbulent Flows.
//     Computers and Fluids, 24(3), 227-238.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     realizableKECoeffs
//     {
//       Cmu         0.09;
//       A0          4.0;
//       C2          1.9;
//       sigmak      1.0;
//       sigmaEps    1.2;
//     }
//   \endverbatim
// SourceFiles
//   realizable_ke.cpp
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class realizableKE
:
  public eddyViscosity<RASModel<BasicTurbulenceModel> >
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmu_;
      dimensionedScalar A0_;
      dimensionedScalar C2_;
      dimensionedScalar sigmak_;
      dimensionedScalar sigmaEps_;
    // Fields
      volScalarField k_;
      volScalarField epsilon_;
 // Protected Member Functions
    tmp<volScalarField> rCmu
    (
      const volTensorField& gradU,
      const volScalarField& S2,
      const volScalarField& magS
    );
    virtual void correctNut
    (
      const volTensorField& gradU,
      const volScalarField& S2,
      const volScalarField& magS
    );
    virtual void correctNut();
    virtual tmp<fvScalarMatrix> kSource() const;
    virtual tmp<fvScalarMatrix> epsilonSource() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("realizableKE");
  // Constructors
    //- Construct from components
    realizableKE
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
  virtual ~realizableKE()
  {}
  // Member Functions
    //- Re-read model coefficients if they have changed
    virtual bool read();
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          "DkEff",
          (this->nut_/sigmak_ + this->nu())
        )
      );
    }
    //- Return the effective diffusivity for epsilon
    tmp<volScalarField> DepsilonEff() const
    {
      return tmp<volScalarField>
      (
        new volScalarField
        (
          "DepsilonEff",
          (this->nut_/sigmaEps_ + this->nu())
        )
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
}  // namespace mousse
#ifdef NoRepository
#   include "realizable_ke.cpp"
#endif
#endif
