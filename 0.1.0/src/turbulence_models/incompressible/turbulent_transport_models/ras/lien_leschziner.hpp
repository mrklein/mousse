// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::incompressible::RASModels::LienLeschziner
// Group
//   grpIcoRASTurbulence
// Description
//   Lien and Leschziner low-Reynolds number k-epsilon turbulence model for
//   incompressible flows.
//   This turbulence model is described in:
//   \verbatim
//     Lien, F. S., & Leschziner, M. A. (1993).
//     A pressure-velocity solution strategy for compressible flow
//     and its application to shock/boundary-layer interaction
//     using second-moment turbulence closure.
//     Journal of fluids engineering, 115(4), 717-725.
//   \endverbatim
//   Implemented according to the specification in:
//   <a href=
//   "http://personalpages.manchester.ac.uk/staff/david.d.apsley/specturb.pdf"
//   >Apsley: Turbulence Models 2002</a>
//   In addition to the low-Reynolds number damping functions support for
//   wall-functions is also included to allow for low- and high-Reynolds number
//   operation.
// SourceFiles
//   lien_leschziner.cpp
#ifndef lien_leschziner_hpp_
#define lien_leschziner_hpp_
#include "turbulent_transport_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace incompressible
{
namespace RASModels
{
class LienLeschziner
:
  public eddyViscosity<incompressible::RASModel>
{
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Ceps1_;
      dimensionedScalar Ceps2_;
      dimensionedScalar sigmak_;
      dimensionedScalar sigmaEps_;
      dimensionedScalar Cmu_;
      dimensionedScalar kappa_;
      dimensionedScalar Anu_;
      dimensionedScalar Aeps_;
      dimensionedScalar AE_;
    // Fields
      volScalarField k_;
      volScalarField epsilon_;
      //- Wall distance
      //  Note: different to wall distance in parent RASModel
      //  which is for near-wall cells only
      const volScalarField& y_;
  // Protected Member Functions
    tmp<volScalarField> fMu() const;
    tmp<volScalarField> f2() const;
    tmp<volScalarField> E(const volScalarField& f2) const;
    virtual void correctNut();
public:
  TypeName("LienLeschziner");
  // Constructors
    //- Construct from components
    LienLeschziner
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
  virtual ~LienLeschziner()
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
