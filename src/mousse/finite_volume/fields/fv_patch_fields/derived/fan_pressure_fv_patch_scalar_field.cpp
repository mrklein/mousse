// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fan_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


namespace mousse {

template<>
const char* NamedEnum
<
  fanPressureFvPatchScalarField::fanFlowDirection,
  2
>::names[] =
{
  "in",
  "out"
};

}

const mousse::NamedEnum
<
  mousse::fanPressureFvPatchScalarField::fanFlowDirection,
  2
> mousse::fanPressureFvPatchScalarField::fanFlowDirectionNames_;


// Constructors 
mousse::fanPressureFvPatchScalarField::fanPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  totalPressureFvPatchScalarField{p, iF},
  fanCurve_{},
  direction_{ffdOut}
{}


mousse::fanPressureFvPatchScalarField::fanPressureFvPatchScalarField
(
  const fanPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  totalPressureFvPatchScalarField{ptf, p, iF, mapper},
  fanCurve_{ptf.fanCurve_},
  direction_{ptf.direction_}
{}


mousse::fanPressureFvPatchScalarField::fanPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  totalPressureFvPatchScalarField{p, iF, dict},
  fanCurve_{dict},
  direction_{fanFlowDirectionNames_.read(dict.lookup("direction"))}
{}


mousse::fanPressureFvPatchScalarField::fanPressureFvPatchScalarField
(
  const fanPressureFvPatchScalarField& pfopsf
)
:
  totalPressureFvPatchScalarField{pfopsf},
  fanCurve_{pfopsf.fanCurve_},
  direction_{pfopsf.direction_}
{}


mousse::fanPressureFvPatchScalarField::fanPressureFvPatchScalarField
(
  const fanPressureFvPatchScalarField& pfopsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  totalPressureFvPatchScalarField{pfopsf, iF},
  fanCurve_{pfopsf.fanCurve_},
  direction_{pfopsf.direction_}
{}


// Member Functions 
void mousse::fanPressureFvPatchScalarField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  // Retrieve flux field
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName());
  const fvsPatchField<scalar>& phip =
    patch().patchField<surfaceScalarField, scalar>(phi);
  int dir = 2*direction_ - 1;
  // Average volumetric flow rate
  scalar volFlowRate = 0;
  if (phi.dimensions() == dimVelocity*dimArea) {
    volFlowRate = dir*gSum(phip);
  } else if (phi.dimensions() == dimVelocity*dimArea*dimDensity) {
    const scalarField& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName());
    volFlowRate = dir*gSum(phip/rhop);
  } else {
    FATAL_ERROR_IN("fanPressureFvPatchScalarField::updateCoeffs()")
      << "dimensions of phi are not correct"
        << "\n    on patch " << patch().name()
        << " of field " << dimensionedInternalField().name()
        << " in file " << dimensionedInternalField().objectPath() << nl
        << exit(FatalError);
  }
  // Pressure drop for this flow rate
  const scalar pdFan = fanCurve_(max(volFlowRate, 0.0));
  totalPressureFvPatchScalarField::updateCoeffs
  (
    p0() - dir*pdFan,
    patch().lookupPatchField<volVectorField, vector>(UName())
  );
}


void mousse::fanPressureFvPatchScalarField::write(Ostream& os) const
{
  totalPressureFvPatchScalarField::write(os);
  fanCurve_.write(os);
  os.writeKeyword("direction")
    << fanFlowDirectionNames_[direction_] << token::END_STATEMENT << nl;
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  fanPressureFvPatchScalarField
);

};
