// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulent_heat_flux_temperature_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"


// Static Data Members
namespace mousse {

// declare specialization within 'Foam' namespace
template<>
const char* NamedEnum
<
  mousse::compressible::
  turbulentHeatFluxTemperatureFvPatchScalarField::heatSourceType,
  2
>::names[] =
{
  "power",
  "flux"
};

}


namespace mousse {
namespace compressible {

// Static Data Members
const NamedEnum
<
  turbulentHeatFluxTemperatureFvPatchScalarField::heatSourceType,
  2
> turbulentHeatFluxTemperatureFvPatchScalarField::heatSourceTypeNames_;


// Constructors 
turbulentHeatFluxTemperatureFvPatchScalarField::
turbulentHeatFluxTemperatureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField{p, iF},
  temperatureCoupledBase{patch(), "undefined", "undefined", "undefined-K"},
  heatSource_{hsPower},
  q_{p.size(), 0.0},
  QrName_{"undefinedQr"}
{}


turbulentHeatFluxTemperatureFvPatchScalarField::
turbulentHeatFluxTemperatureFvPatchScalarField
(
  const turbulentHeatFluxTemperatureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedGradientFvPatchScalarField{ptf, p, iF, mapper},
  temperatureCoupledBase{patch(), ptf},
  heatSource_{ptf.heatSource_},
  q_{ptf.q_, mapper},
  QrName_{ptf.QrName_}
{}


turbulentHeatFluxTemperatureFvPatchScalarField::
turbulentHeatFluxTemperatureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedGradientFvPatchScalarField{p, iF},
  temperatureCoupledBase{patch(), dict},
  heatSource_{heatSourceTypeNames_.read(dict.lookup("heatSource"))},
  q_{"q", dict, p.size()},
  QrName_{dict.lookupOrDefault<word>("Qr", "none")}
{
  if (dict.found("value") && dict.found("gradient")) {
    fvPatchField<scalar>::operator=(Field<scalar>{"value", dict, p.size()});
    gradient() = Field<scalar>{"gradient", dict, p.size()};
  } else {
    // Still reading so cannot yet evaluate. Make up a value.
    fvPatchField<scalar>::operator=(patchInternalField());
    gradient() = 0.0;
  }
}


turbulentHeatFluxTemperatureFvPatchScalarField::
turbulentHeatFluxTemperatureFvPatchScalarField
(
  const turbulentHeatFluxTemperatureFvPatchScalarField& thftpsf
)
:
  fixedGradientFvPatchScalarField{thftpsf},
  temperatureCoupledBase{patch(), thftpsf},
  heatSource_{thftpsf.heatSource_},
  q_{thftpsf.q_},
  QrName_{thftpsf.QrName_}
{}


turbulentHeatFluxTemperatureFvPatchScalarField::
turbulentHeatFluxTemperatureFvPatchScalarField
(
  const turbulentHeatFluxTemperatureFvPatchScalarField& thftpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedGradientFvPatchScalarField{thftpsf, iF},
  temperatureCoupledBase{patch(), thftpsf},
  heatSource_{thftpsf.heatSource_},
  q_{thftpsf.q_},
  QrName_{thftpsf.QrName_}
{}


// Member Functions 
void turbulentHeatFluxTemperatureFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedGradientFvPatchScalarField::autoMap(m);
  q_.autoMap(m);
}


void turbulentHeatFluxTemperatureFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  fixedGradientFvPatchScalarField::rmap(ptf, addr);
  const turbulentHeatFluxTemperatureFvPatchScalarField& thftptf =
    refCast<const turbulentHeatFluxTemperatureFvPatchScalarField>
    (
      ptf
    );
  q_.rmap(thftptf.q_, addr);
}


void turbulentHeatFluxTemperatureFvPatchScalarField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const scalarField& Tp = *this;
  scalarField qr(this->size(), 0.0);
  //- Qr is negative going into the domain
  if (QrName_ != "none") {
    qr = patch().lookupPatchField<volScalarField, scalar>(QrName_);
  }
  switch (heatSource_) {
    case hsPower:
      {
        const scalar Ap = gSum(patch().magSf());
        gradient() = (q_/Ap + qr)/kappa(Tp);
        break;
      }
    case hsFlux:
      {
        gradient() = (q_ + qr)/kappa(Tp);
        break;
      }
    default:
      {
        FATAL_ERROR_IN
        (
          "turbulentHeatFluxTemperatureFvPatchScalarField::updateCoeffs()"
        )
        << "Unknown heat source type. Valid types are: "
        << heatSourceTypeNames_ << nl << exit(FatalError);
      }
  }
  fixedGradientFvPatchScalarField::updateCoeffs();
}


void turbulentHeatFluxTemperatureFvPatchScalarField::write
(
  Ostream& os
) const
{
  fixedGradientFvPatchScalarField::write(os);
  os.writeKeyword("heatSource") << heatSourceTypeNames_[heatSource_]
    << token::END_STATEMENT << nl;
  temperatureCoupledBase::write(os);
  q_.writeEntry("q", os);
  os.writeKeyword("Qr")<< QrName_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}


MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  turbulentHeatFluxTemperatureFvPatchScalarField
);

}  // namespace compressible
}  // namespace mousse

