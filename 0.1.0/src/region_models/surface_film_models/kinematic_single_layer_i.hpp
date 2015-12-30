// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_thermo_model.hpp"
#include "surface_interpolate.hpp"
#include "fvc_surface_integrate.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
inline const Switch& kinematicSingleLayer::momentumPredictor() const
{
  return momentumPredictor_;
}
inline label kinematicSingleLayer::nOuterCorr() const
{
  return nOuterCorr_;
}
inline label kinematicSingleLayer::nCorr() const
{
  return nCorr_;
}
inline label kinematicSingleLayer::nNonOrthCorr() const
{
  return nNonOrthCorr_;
}
inline const dimensionedScalar& kinematicSingleLayer::deltaSmall() const
{
  return deltaSmall_;
}
inline const volScalarField& kinematicSingleLayer::mu() const
{
  return mu_;
}
inline const volScalarField& kinematicSingleLayer::sigma() const
{
  return sigma_;
}
inline const volScalarField& kinematicSingleLayer::delta() const
{
  return delta_;
}
inline const volScalarField& kinematicSingleLayer::alpha() const
{
  return alpha_;
}
inline volVectorField& kinematicSingleLayer::USpPrimary()
{
  return USpPrimary_;
}
inline volScalarField& kinematicSingleLayer::pSpPrimary()
{
  return pSpPrimary_;
}
inline volScalarField& kinematicSingleLayer::rhoSpPrimary()
{
  return rhoSpPrimary_;
}
inline volVectorField& kinematicSingleLayer::USp()
{
  return USp_;
}
inline volScalarField& kinematicSingleLayer::pSp()
{
  return pSp_;
}
inline volScalarField& kinematicSingleLayer::rhoSp()
{
  return rhoSp_;
}
inline const volVectorField& kinematicSingleLayer::USp() const
{
  return USp_;
}
inline const volScalarField& kinematicSingleLayer::pSp() const
{
  return pSp_;
}
inline const volScalarField& kinematicSingleLayer::rhoSp() const
{
  return rhoSp_;
}
inline const volVectorField& kinematicSingleLayer::UPrimary() const
{
  return UPrimary_;
}
inline const volScalarField& kinematicSingleLayer::pPrimary() const
{
  return pPrimary_;
}
inline const volScalarField& kinematicSingleLayer::rhoPrimary() const
{
  return rhoPrimary_;
}
inline const volScalarField& kinematicSingleLayer::muPrimary() const
{
  return muPrimary_;
}
inline const filmThermoModel& kinematicSingleLayer::filmThermo() const
{
  return filmThermo_();
}
inline injectionModelList& kinematicSingleLayer::injection()
{
  return injection_;
}
inline const filmTurbulenceModel& kinematicSingleLayer::turbulence() const
{
  return turbulence_();
}
inline tmp<volScalarField> kinematicSingleLayer::mass() const
{
  return rho_*delta_*magSf();
}
inline tmp<volScalarField> kinematicSingleLayer::netMass() const
{
  return
    fvc::surfaceSum(pos(phi_)*phi_/(fvc::interpolate(delta_) + deltaSmall_))
   *time().deltaT()
   + rho_*delta_*magSf();
}
inline tmp<volScalarField> kinematicSingleLayer::deltaMass() const
{
  return rhoSp_*magSf()*time().deltaT();
}
inline tmp<volScalarField> kinematicSingleLayer::gNorm() const
{
  tmp<volScalarField> tgNorm
  (
    new volScalarField
    (
      IOobject
      (
        "gNorm",
        time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      g_ & nHat(),
      zeroGradientFvPatchScalarField::typeName
    )
  );
  return tgNorm;
}
inline tmp<volScalarField> kinematicSingleLayer::gNormClipped() const
{
  tmp<volScalarField> tgNormClipped
  (
    new volScalarField
    (
      IOobject
      (
        "gNormClipped",
        time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      g_ & nHat(),
      zeroGradientFvPatchScalarField::typeName
    )
  );
  volScalarField& gNormClipped = tgNormClipped();
  gNormClipped.min(0.0);
  return tgNormClipped;
}
inline tmp<volVectorField> kinematicSingleLayer::gTan() const
{
  tmp<volVectorField> tgTan
  (
    new volVectorField
    (
      IOobject
      (
        "gTan",
        time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      g_ - nHat()*gNorm(),
      zeroGradientFvPatchVectorField::typeName
    )
  );
  return tgTan;
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
