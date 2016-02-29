#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_LAUNDER_SHARMA_KE_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_LAUNDER_SHARMA_KE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::LaunderSharmaKE
// Group
//   grpRASTurbulence
// Description
//   Launder and Sharma low-Reynolds k-epsilon turbulence model for
//   incompressible and compressible and combusting flows including
//   rapid distortion theory (RDT) based compression term.
//   References:
//   \verbatim
//     Launder, B. E., & Sharma, B. I. (1974).
//     Application of the energy-dissipation model of turbulence to the
//     calculation of flow near a spinning disc.
//     Letters in heat and mass transfer, 1(2), 131-137.
//     For the RDT-based compression term:
//     El Tahry, S. H. (1983).
//     k-epsilon equation for compressible reciprocating engine flows.
//     Journal of Energy, 7(4), 345-353.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     LaunderSharmaKECoeffs
//     {
//       Cmu         0.09;
//       C1          1.44;
//       C2          1.92;
//       C3          -0.33;
//       alphah      1.0;    // only for compressible
//       alphahk     1.0;    // only for compressible
//       alphaEps    0.76923;
//     }
//   \endverbatim
// SourceFiles
//   launder_sharma_ke.cpp
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class LaunderSharmaKE
:
  public eddyViscosity<RASModel<BasicTurbulenceModel> >
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmu_;
      dimensionedScalar C1_;
      dimensionedScalar C2_;
      dimensionedScalar C3_;
      dimensionedScalar sigmak_;
      dimensionedScalar sigmaEps_;
    // Fields
      volScalarField k_;
      volScalarField epsilon_;
  // Private Member Functions
    tmp<volScalarField> fMu() const;
    tmp<volScalarField> f2() const;
    virtual void correctNut();
    virtual tmp<fvScalarMatrix> kSource() const;
    virtual tmp<fvScalarMatrix> epsilonSource() const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("LaunderSharmaKE");
  // Constructors
    //- Construct from components
    LaunderSharmaKE
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
    LaunderSharmaKE(const LaunderSharmaKE&) = delete;
    LaunderSharmaKE& operator=(const LaunderSharmaKE&) = delete;
  //- Destructor
  virtual ~LaunderSharmaKE()
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
#   include "launder_sharma_ke.cpp"
#endif
#endif
