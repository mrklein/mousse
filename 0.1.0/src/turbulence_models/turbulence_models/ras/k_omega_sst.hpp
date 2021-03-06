#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_K_OMEGA_SST_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_K_OMEGA_SST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::kOmegaSST
// Group
//   grpRASTurbulence
// Description
//   Implementation of the k-omega-SST turbulence model for
//   incompressible and compressible flows.
//   Turbulence model described in:
//   \verbatim
//     Menter, F. R. & Esch, T. (2001).
//     Elements of Industrial Heat Transfer Prediction.
//     16th Brazilian Congress of Mechanical Engineering (COBEM).
//   \endverbatim
//   with updated coefficients from
//   \verbatim
//     Menter, F. R., Kuntz, M., and Langtry, R. (2003).
//     Ten Years of Industrial Experience with the SST Turbulence Model.
//     Turbulence, Heat and Mass Transfer 4, ed: K. Hanjalic, Y. Nagano,
//     & M. Tummers, Begell House, Inc., 625 - 632.
//   \endverbatim
//   but with the consistent production terms from the 2001 paper as form in the
//   2003 paper is a typo, see
//   \verbatim
//     http://turbmodels.larc.nasa.gov/sst.html
//   \endverbatim
//   and the addition of the optional F3 term for rough walls from
//   \verbatim
//     Hellsten, A. (1998).
//     "Some Improvements in Menter’s k-omega-SST turbulence model"
//     29th AIAA Fluid Dynamics Conference, AIAA-98-2554.
//   \endverbatim
//   Note that this implementation is written in terms of alpha diffusion
//   coefficients rather than the more traditional sigma (alpha = 1/sigma) so
//   that the blending can be applied to all coefficuients in a consistent
//   manner.  The paper suggests that sigma is blended but this would not be
//   consistent with the blending of the k-epsilon and k-omega models.
//   Also note that the error in the last term of equation (2) relating to
//   sigma has been corrected.
//   Wall-functions are applied in this implementation by using equations (14)
//   to specify the near-wall omega as appropriate.
//   The blending functions (15) and (16) are not currently used because of the
//   uncertainty in their origin, range of applicability and that if y+ becomes
//   sufficiently small blending u_tau in this manner clearly becomes nonsense.
//   The default model coefficients are
//   \verbatim
//     kOmegaSSTCoeffs
//     {
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
//     }
//   \endverbatim

#include "ras_model.hpp"
#include "eddy_viscosity.hpp"


namespace mousse {
namespace RASModels {

template<class BasicTurbulenceModel>
class kOmegaSST
:
  public eddyViscosity<RASModel<BasicTurbulenceModel>>
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar alphaK1_;
      dimensionedScalar alphaK2_;
      dimensionedScalar alphaOmega1_;
      dimensionedScalar alphaOmega2_;
      dimensionedScalar gamma1_;
      dimensionedScalar gamma2_;
      dimensionedScalar beta1_;
      dimensionedScalar beta2_;
      dimensionedScalar betaStar_;
      dimensionedScalar a1_;
      dimensionedScalar b1_;
      dimensionedScalar c1_;
      Switch F3_;
    // Fields
      //- Wall distance
      //  Note: different to wall distance in parent RASModel
      //  which is for near-wall cells only
      const volScalarField& y_;
      volScalarField k_;
      volScalarField omega_;
  // Private Member Functions
    tmp<volScalarField> F1(const volScalarField& CDkOmega) const;
    tmp<volScalarField> F2() const;
    tmp<volScalarField> F3() const;
    tmp<volScalarField> F23() const;
    tmp<volScalarField> blend
    (
      const volScalarField& F1,
      const dimensionedScalar& psi1,
      const dimensionedScalar& psi2
    ) const
    {
      return F1*(psi1 - psi2) + psi2;
    }
    tmp<volScalarField> alphaK(const volScalarField& F1) const
    {
      return blend(F1, alphaK1_, alphaK2_);
    }
    tmp<volScalarField> alphaOmega(const volScalarField& F1) const
    {
      return blend(F1, alphaOmega1_, alphaOmega2_);
    }
    tmp<volScalarField> beta(const volScalarField& F1) const
    {
      return blend(F1, beta1_, beta2_);
    }
    tmp<volScalarField> gamma(const volScalarField& F1) const
    {
      return blend(F1, gamma1_, gamma2_);
    }
    void correctNut(const volScalarField& S2);
  // Protected Member Functions
    virtual void correctNut();
    virtual tmp<fvScalarMatrix> kSource() const;
    virtual tmp<fvScalarMatrix> omegaSource() const;
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
  TYPE_NAME("kOmegaSST");
  // Constructors
    //- Construct from components
    kOmegaSST
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
    kOmegaSST(const kOmegaSST&) = delete;
    kOmegaSST& operator=(const kOmegaSST&) = delete;
  //- Destructor
  virtual ~kOmegaSST()
  {}
  // Member Functions
    //- Re-read model coefficients if they have changed
    virtual bool read();
    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff(const volScalarField& F1) const
    {
      return
        tmp<volScalarField>
        {
          new volScalarField{"DkEff", alphaK(F1)*this->nut_ + this->nu()}
        };
    }
    //- Return the effective diffusivity for omega
    tmp<volScalarField> DomegaEff(const volScalarField& F1) const
    {
      return
        tmp<volScalarField>
        {
          new volScalarField
          {
            "DomegaEff",
            alphaOmega(F1)*this->nut_ + this->nu()
          }
        };
    }
    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const
    {
      return k_;
    }
    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> epsilon() const
    {
       return
         tmp<volScalarField>
         {
           new volScalarField
           {
             {
               "epsilon",
               this->mesh_.time().timeName(),
               this->mesh_
             },
             betaStar_*k_*omega_,
             omega_.boundaryField().types()
           }
         };
    }
    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> omega() const
    {
      return omega_;
    }
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};

}  // namespace RASModels
}  // namespace mousse

#include "k_omega_sst.ipp"

#endif
