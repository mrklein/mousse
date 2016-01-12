// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::SpalartAllmaras
// Group
//   grpRASTurbulence
// Description
//   Spalart-Allmaras one-eqn mixing-length model for incompressible and
//   compressible external flows.
//   Reference:
//   \verbatim
//     Spalart, P.R., & Allmaras, S.R. (1994).
//     A one-equation turbulence model for aerodynamic flows.
//     La Recherche Aerospatiale, 1, 5-21.
//   \endverbatim
//   The model is implemented without the trip-term and hence the ft2 term is
//   not needed.
//   It is necessary to limit the Stilda generation term as the model generates
//   unphysical results if this term becomes negative which occurs for complex
//   flow.  Several approaches have been proposed to limit Stilda but it is not
//   clear which is the most appropriate.  Here the limiter proposed by Spalart
//   is implemented in which Stilda is clipped at Cs*Omega with the default value
//   of Cs = 0.3.
//   The default model coefficients are
//   \verbatim
//     SpalartAllmarasCoeffs
//     {
//       Cb1         0.1355;
//       Cb2         0.622;
//       Cw2         0.3;
//       Cw3         2.0;
//       Cv1         7.1;
//       Cs          0.3;
//       sigmaNut    0.66666;
//       kappa       0.41;
//     }
//   \endverbatim
// SourceFiles
//   spalart_allmaras.cpp
#ifndef spalart_allmaras_hpp_
#define spalart_allmaras_hpp_
#include "ras_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace RASModels
{
template<class BasicTurbulenceModel>
class SpalartAllmaras
:
  public eddyViscosity<RASModel<BasicTurbulenceModel> >
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar sigmaNut_;
      dimensionedScalar kappa_;
      dimensionedScalar Cb1_;
      dimensionedScalar Cb2_;
      dimensionedScalar Cw1_;
      dimensionedScalar Cw2_;
      dimensionedScalar Cw3_;
      dimensionedScalar Cv1_;
      dimensionedScalar Cs_;
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
    tmp<volScalarField> Stilda
    (
      const volScalarField& chi,
      const volScalarField& fv1
    ) const;
    tmp<volScalarField> fw(const volScalarField& Stilda) const;
    void correctNut(const volScalarField& fv1);
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("SpalartAllmaras");
  // Constructors
    //- Construct from components
    SpalartAllmaras
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
    SpalartAllmaras(const SpalartAllmaras&) = delete;
    SpalartAllmaras& operator=(const SpalartAllmaras&) = delete;
  //- Destructor
  virtual ~SpalartAllmaras()
  {}
  // Member Functions
    //- Read RASProperties dictionary
    virtual bool read();
    //- Return the effective diffusivity for nuTilda
    tmp<volScalarField> DnuTildaEff() const;
    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const;
    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> epsilon() const;
    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();
};
}  // namespace RASModels
}  // namespace mousse
#ifdef NoRepository
#   include "spalart_allmaras.cpp"
#endif
#endif
