#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_K_OMEGA_SSTSAS_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_K_OMEGA_SSTSAS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::kOmegaSSTSAS
// Group
//   grpLESTurbulence
// Description
//   Scale-adaptive URAS model based on the k-omega-SST RAS model.
//   References:
//   \verbatim
//     Egorov, Y., & Menter F.R. (2008).
//     Development and Application of SST-SAS Model in the DESIDER Project.
//     Advances in Hybrid RANS-LES Modelling,
//     Notes on Num. Fluid Mech. And Multidisciplinary Design,
//     Volume 97, 261-270.
//   \endverbatim
//   The model coefficients are
//   \verbatim
//     kOmegaSSTSASCoeffs
//     {
//       // Default SST coefficients
//       alphaK1     0.85;
//       alphaK2     1.0;
//       alphaOmega1 0.5;
//       alphaOmega2 0.856;
//       beta1       0.075;
//       beta2       0.0828;
//       betaStar    0.09;
//       gamma1      5/9;
//       gamma2      0.44;
//       a1          0.31;
//       b1          1.0;
//       c1          10.0;
//       F3          no;
//       // Default SAS coefficients
//       Cs          0.11;
//       kappa       0.41;
//       zeta2       3.51;
//       sigmaPhi    2.0/3.0;
//       C           2;
//       // Delta must be specified for SAS e.g.
//       delta cubeRootVol;
//       cubeRootVolCoeffs
//       {}
//     }
//   \endverbatim
// SourceFiles
//   k_omega_sstsas.cpp
#include "k_omega_sst.hpp"
#include "les_delta.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class kOmegaSSTSAS
:
  public kOmegaSST<BasicTurbulenceModel>
{
protected:
  // Protected data
    // Model constants
      dimensionedScalar Cs_;
      dimensionedScalar kappa_;
      dimensionedScalar zeta2_;
      dimensionedScalar sigmaPhi_;
      dimensionedScalar C_;
    // Fields
      //- Run-time selectable delta model
      autoPtr<mousse::LESdelta> delta_;
  // Protected Member Functions
    //- SAS omega source
    virtual tmp<fvScalarMatrix> Qsas
    (
      const volScalarField& S2,
      const volScalarField& gamma,
      const volScalarField& beta
    ) const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("kOmegaSSTSAS");
  // Constructors
    //- Construct from components
    kOmegaSSTSAS
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
    kOmegaSSTSAS(const kOmegaSSTSAS&) = delete;
    kOmegaSSTSAS& operator=(const kOmegaSSTSAS&) = delete;
  //- Destructor
  virtual ~kOmegaSSTSAS()
  {}
  // Member Functions
    //- Re-read model coefficients if they have changed
    virtual bool read();
    //- Access function to filter width
    inline const volScalarField& delta() const
    {
      return delta_();
    }
};
}  // namespace RASModels
}  // namespace mousse
#ifdef NoRepository
#   include "k_omega_sstsas.cpp"
#endif
#endif
