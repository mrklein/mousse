// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "external_wall_heat_flux_temperature_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "mapped_patch_base.hpp"
namespace mousse
{
  template<>
  const char*
  NamedEnum
  <
    externalWallHeatFluxTemperatureFvPatchScalarField::operationMode,
    3
  >::names[] =
  {
    "fixed_heat_flux",
    "fixed_heat_transfer_coefficient",
    "unknown"
  };
}  // namespace mousse
const mousse::NamedEnum
<
  mousse::externalWallHeatFluxTemperatureFvPatchScalarField::operationMode,
  3
> mousse::externalWallHeatFluxTemperatureFvPatchScalarField::operationModeNames;
// Constructors 
mousse::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField(p, iF),
  temperatureCoupledBase(patch(), "undefined", "undefined", "undefined-K"),
  mode_(unknown),
  q_(p.size(), 0.0),
  h_(p.size(), 0.0),
  Ta_(p.size(), 0.0),
  QrPrevious_(p.size()),
  QrRelaxation_(1),
  QrName_("undefined-Qr"),
  thicknessLayers_(),
  kappaLayers_()
{
  refValue() = 0.0;
  refGrad() = 0.0;
  valueFraction() = 1.0;
}
mousse::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
  const externalWallHeatFluxTemperatureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField(ptf, p, iF, mapper),
  temperatureCoupledBase(patch(), ptf),
  mode_(ptf.mode_),
  q_(ptf.q_, mapper),
  h_(ptf.h_, mapper),
  Ta_(ptf.Ta_, mapper),
  QrPrevious_(ptf.QrPrevious_, mapper),
  QrRelaxation_(ptf.QrRelaxation_),
  QrName_(ptf.QrName_),
  thicknessLayers_(ptf.thicknessLayers_),
  kappaLayers_(ptf.kappaLayers_)
{}
mousse::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField(p, iF),
  temperatureCoupledBase(patch(), dict),
  mode_(unknown),
  q_(p.size(), 0.0),
  h_(p.size(), 0.0),
  Ta_(p.size(), 0.0),
  QrPrevious_(p.size(), 0.0),
  QrRelaxation_(dict.lookupOrDefault<scalar>("relaxation", 1)),
  QrName_(dict.lookupOrDefault<word>("Qr", "none")),
  thicknessLayers_(),
  kappaLayers_()
{
  if (dict.found("q") && !dict.found("h") && !dict.found("Ta"))
  {
    mode_ = fixedHeatFlux;
    q_ = scalarField("q", dict, p.size());
  }
  else if (dict.found("h") && dict.found("Ta") && !dict.found("q"))
  {
    mode_ = fixedHeatTransferCoeff;
    h_ = scalarField("h", dict, p.size());
    Ta_ = scalarField("Ta", dict, p.size());
    if (dict.found("thicknessLayers"))
    {
      dict.lookup("thicknessLayers") >> thicknessLayers_;
      dict.lookup("kappaLayers") >> kappaLayers_;
    }
  }
  else
  {
    FatalErrorIn
    (
      "externalWallHeatFluxTemperatureFvPatchScalarField::"
      "externalWallHeatFluxTemperatureFvPatchScalarField\n"
      "(\n"
      "    const fvPatch& p,\n"
      "    const DimensionedField<scalar, volMesh>& iF,\n"
      "    const dictionary& dict\n"
      ")\n"
    )   << "\n patch type '" << p.type()
      << "' either q or h and Ta were not found '"
      << "\n for patch " << p.name()
      << " of field " << dimensionedInternalField().name()
      << " in file " << dimensionedInternalField().objectPath()
      << exit(FatalError);
  }
  fvPatchScalarField::operator=(scalarField("value", dict, p.size()));
  if (dict.found("QrPrevious"))
  {
    QrPrevious_ = scalarField("QrPrevious", dict, p.size());
  }
  if (dict.found("refValue"))
  {
    // Full restart
    refValue() = scalarField("refValue", dict, p.size());
    refGrad() = scalarField("refGradient", dict, p.size());
    valueFraction() = scalarField("valueFraction", dict, p.size());
  }
  else
  {
    // Start from user entered data. Assume fixedValue.
    refValue() = *this;
    refGrad() = 0.0;
    valueFraction() = 1.0;
  }
}
mousse::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
  const externalWallHeatFluxTemperatureFvPatchScalarField& tppsf
)
:
  mixedFvPatchScalarField(tppsf),
  temperatureCoupledBase(tppsf),
  mode_(tppsf.mode_),
  q_(tppsf.q_),
  h_(tppsf.h_),
  Ta_(tppsf.Ta_),
  QrPrevious_(tppsf.QrPrevious_),
  QrRelaxation_(tppsf.QrRelaxation_),
  QrName_(tppsf.QrName_),
  thicknessLayers_(tppsf.thicknessLayers_),
  kappaLayers_(tppsf.kappaLayers_)
{}
mousse::externalWallHeatFluxTemperatureFvPatchScalarField::
externalWallHeatFluxTemperatureFvPatchScalarField
(
  const externalWallHeatFluxTemperatureFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField(tppsf, iF),
  temperatureCoupledBase(patch(), tppsf),
  mode_(tppsf.mode_),
  q_(tppsf.q_),
  h_(tppsf.h_),
  Ta_(tppsf.Ta_),
  QrPrevious_(tppsf.QrPrevious_),
  QrRelaxation_(tppsf.QrRelaxation_),
  QrName_(tppsf.QrName_),
  thicknessLayers_(tppsf.thicknessLayers_),
  kappaLayers_(tppsf.kappaLayers_)
{}
// Member Functions 
void mousse::externalWallHeatFluxTemperatureFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  mixedFvPatchScalarField::autoMap(m);
  q_.autoMap(m);
  h_.autoMap(m);
  Ta_.autoMap(m);
}
void mousse::externalWallHeatFluxTemperatureFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  mixedFvPatchScalarField::rmap(ptf, addr);
  const externalWallHeatFluxTemperatureFvPatchScalarField& tiptf =
    refCast<const externalWallHeatFluxTemperatureFvPatchScalarField>(ptf);
  q_.rmap(tiptf.q_, addr);
  h_.rmap(tiptf.h_, addr);
  Ta_.rmap(tiptf.Ta_, addr);
}
void mousse::externalWallHeatFluxTemperatureFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const scalarField Tp(*this);
  scalarField hp(patch().size(), 0.0);
  scalarField Qr(Tp.size(), 0.0);
  if (QrName_ != "none")
  {
    Qr = patch().lookupPatchField<volScalarField, scalar>(QrName_);
    Qr = QrRelaxation_*Qr + (1.0 - QrRelaxation_)*QrPrevious_;
    QrPrevious_ = Qr;
  }
  switch (mode_)
  {
    case fixedHeatFlux:
    {
      refGrad() = (q_ + Qr)/kappa(Tp);
      refValue() = 0.0;
      valueFraction() = 0.0;
      break;
    }
    case fixedHeatTransferCoeff:
    {
      scalar totalSolidRes = 0.0;
      if (thicknessLayers_.size() > 0)
      {
        forAll (thicknessLayers_, iLayer)
        {
          const scalar l = thicknessLayers_[iLayer];
          if (kappaLayers_[iLayer] > 0.0)
          {
            totalSolidRes += l/kappaLayers_[iLayer];
          }
        }
      }
      hp = 1.0/(1.0/h_ + totalSolidRes);
      Qr /= Tp;
      refGrad() = 0.0;
      refValue() = hp*Ta_/(hp - Qr);
      valueFraction() =
        (hp - Qr)/((hp - Qr) + kappa(Tp)*patch().deltaCoeffs());
      break;
    }
    default:
    {
      FatalErrorIn
      (
        "externalWallHeatFluxTemperatureFvPatchScalarField"
        "::updateCoeffs()"
      )   << "Illegal heat flux mode " << operationModeNames[mode_]
        << exit(FatalError);
    }
  }
  mixedFvPatchScalarField::updateCoeffs();
  if (debug)
  {
    scalar Q = gSum(kappa(Tp)*patch().magSf()*snGrad());
    Info<< patch().boundaryMesh().mesh().name() << ':'
      << patch().name() << ':'
      << this->dimensionedInternalField().name() << " :"
      << " heat transfer rate:" << Q
      << " walltemperature "
      << " min:" << gMin(*this)
      << " max:" << gMax(*this)
      << " avg:" << gAverage(*this)
      << endl;
  }
}
void mousse::externalWallHeatFluxTemperatureFvPatchScalarField::write
(
  Ostream& os
) const
{
  mixedFvPatchScalarField::write(os);
  temperatureCoupledBase::write(os);
  QrPrevious_.writeEntry("QrPrevious", os);
  os.writeKeyword("Qr")<< QrName_ << token::END_STATEMENT << nl;
  os.writeKeyword("relaxation")<< QrRelaxation_
    << token::END_STATEMENT << nl;
  switch (mode_)
  {
    case fixedHeatFlux:
    {
      q_.writeEntry("q", os);
      break;
    }
    case fixedHeatTransferCoeff:
    {
      h_.writeEntry("h", os);
      Ta_.writeEntry("Ta", os);
      thicknessLayers_.writeEntry("thicknessLayers", os);
      kappaLayers_.writeEntry("kappaLayers", os);
      break;
    }
    default:
    {
      FatalErrorIn
      (
        "void externalWallHeatFluxTemperatureFvPatchScalarField::write"
        "("
          "Ostream& os"
        ") const"
      )   << "Illegal heat flux mode " << operationModeNames[mode_]
        << abort(FatalError);
    }
  }
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    externalWallHeatFluxTemperatureFvPatchScalarField
  );
}
