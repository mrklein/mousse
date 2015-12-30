// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermo_single_layer.hpp"
#include "film_radiation_model.hpp"
#include "heat_transfer_model.hpp"
#include "phase_change_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
inline const SLGThermo& thermoSingleLayer::thermo() const
{
  return thermo_;
}
inline tmp<scalarField> thermoSingleLayer::hs
(
  const scalarField& T,
  const label patchI
) const
{
  const scalarField& Cp = Cp_.boundaryField()[patchI];
  return Cp*(T - 298.15);
}
inline tmp<volScalarField> thermoSingleLayer::hs
(
  const volScalarField& T
) const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        "hs(" + T.name() + ")",
        time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      Cp_*(T - (dimensionedScalar("Tstd", dimTemperature, 298.15))),
      zeroGradientFvPatchScalarField::typeName
    )
  );
}
inline tmp<volScalarField> thermoSingleLayer::T
(
  const volScalarField& hs
) const
{
  tmp<volScalarField> tT
  (
    new volScalarField
    (
      IOobject
      (
        "T(" + hs.name() + ")",
        time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      hs/Cp_ + dimensionedScalar("Tstd", dimTemperature, 298.15),
      zeroGradientFvPatchScalarField::typeName
    )
  );
  tT().min(Tmax_);
  tT().max(Tmin_);
  return tT;
}
inline const volScalarField& thermoSingleLayer::hsSp() const
{
  return hsSp_;
}
inline const volScalarField& thermoSingleLayer::hsSpPrimary() const
{
  return hsSpPrimary_;
}
inline const volScalarField& thermoSingleLayer::TPrimary() const
{
  return TPrimary_;
}
inline const PtrList<volScalarField>& thermoSingleLayer::YPrimary() const
{
  return YPrimary_;
}
inline const heatTransferModel& thermoSingleLayer::htcs() const
{
  return htcs_();
}
inline const heatTransferModel& thermoSingleLayer::htcw() const
{
  return htcw_();
}
inline const phaseChangeModel& thermoSingleLayer::phaseChange() const
{
  return phaseChange_();
}
inline const filmRadiationModel& thermoSingleLayer::radiation() const
{
  return radiation_();
}
inline tmp<scalarField> thermoSingleLayer::Qconvw(const label patchI) const
{
  const scalarField htc(htcw_->h()().boundaryField()[patchI]);
  const scalarField& Tp = T_.boundaryField()[patchI];
  const scalarField& Twp = Tw_.boundaryField()[patchI];
  return htc*(Tp - Twp);
}
inline tmp<scalarField> thermoSingleLayer::Qconvp(const label patchI) const
{
  const scalarField htc(htcs_->h()().boundaryField()[patchI]);
  const scalarField& Tp = T_.boundaryField()[patchI];
  const scalarField& Tpp = TPrimary_.boundaryField()[patchI];
  return htc*(Tp - Tpp);
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
