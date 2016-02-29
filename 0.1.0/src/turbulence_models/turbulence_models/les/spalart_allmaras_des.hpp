#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_SPALART_ALLMARAS_DES_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_SPALART_ALLMARAS_DES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::SpalartAllmarasDES
// Group
//   grpDESTurbulence
// Description
//   SpalartAllmarasDES DES turbulence model for incompressible and
//   compressible flows
//   Reference:
//   \verbatim
//     Spalart, P. R., Jou, W. H., Strelets, M., & Allmaras, S. R. (1997).
//     Comments on the feasibility of LES for wings, and on a hybrid
//     RANS/LES approach.
//     Advances in DNS/LES, 1, 4-8.
//   \endverbatim
// SourceFiles
//   spalart_allmaras_des.cpp
#include "les_eddy_viscosity.hpp"
namespace mousse
{
namespace LESModels
{
template<class BasicTurbulenceModel>
class SpalartAllmarasDES
:
  public LESeddyViscosity<BasicTurbulenceModel>
{
protected:
  // Protected data
    // Model constants
      dimensionedScalar sigmaNut_;
      dimensionedScalar kappa_;
      dimensionedScalar Cb1_;
      dimensionedScalar Cb2_;
      dimensionedScalar Cw1_;
      dimensionedScalar Cw2_;
      dimensionedScalar Cw3_;
      dimensionedScalar Cv1_;
      dimensionedScalar Cs_;
      dimensionedScalar CDES_;
      dimensionedScalar ck_;
    // Fields
      volScalarField nuTilda_;
      //- Wall distance
      //  Note: different to wall distance in parent RASModel
      //  which is for near-wall cells only
      const volScalarField& y_;
  // Protected Member Functions
    tmp<volScalarField> chi() const;
    tmp<volScalarField> fv1(const volScalarField& chi) const;
    tmp<volScalarField> fv2
    (
      const volScalarField& chi,
      const volScalarField& fv1
    ) const;
    tmp<volScalarField> S(const volTensorField& gradU) const;
    tmp<volScalarField> Omega(const volTensorField& gradU) const;
    tmp<volScalarField> Stilda
    (
      const volScalarField& chi,
      const volScalarField& fv1,
      const volScalarField& Omega,
      const volScalarField& dTilda
    ) const;
    tmp<volScalarField> r
    (
      const volScalarField& nur,
      const volScalarField& Omega,
      const volScalarField& dTilda
    ) const;
    tmp<volScalarField> fw
    (
      const volScalarField& Omega,
      const volScalarField& dTilda
    ) const;
    //- Length scale
    virtual tmp<volScalarField> dTilda
    (
      const volScalarField& chi,
      const volScalarField& fv1,
      const volTensorField& gradU
    ) const;
    void correctNut(const volScalarField& fv1);
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("SpalartAllmarasDES");
  // Constructors
    //- Construct from components
    SpalartAllmarasDES
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
    SpalartAllmarasDES(const SpalartAllmarasDES&) = delete;
    SpalartAllmarasDES& operator=(const SpalartAllmarasDES&) = delete;
  //- Destructor
  virtual ~SpalartAllmarasDES()
  {}
  // Member Functions
    //- Read LESProperties dictionary
    virtual bool read();
    //- Return the effective diffusivity for nuTilda
    tmp<volScalarField> DnuTildaEff() const;
    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const;
    tmp<volScalarField> nuTilda() const
    {
      return nuTilda_;
    }
    //- Return the LES field indicator
    virtual tmp<volScalarField> LESRegion() const;
    //- Correct nuTilda and related properties
    virtual void correct();
};
}  // namespace LESModels
}  // namespace mousse
#ifdef NoRepository
#   include "spalart_allmaras_des.cpp"
#endif
#endif
