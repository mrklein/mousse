// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "prgh_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "uniform_dimensioned_fields.hpp"

// Constructors 
mousse::prghPressureFvPatchScalarField::
prghPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  rhoName_{"rho"},
  p_{p.size(), 0.0}
{}

mousse::prghPressureFvPatchScalarField::
prghPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")},
  p_{"p", dict, p.size()}
{
  if (dict.found("value"))
  {
    fvPatchScalarField::operator=
    (
      scalarField{"value", dict, p.size()}
    );
  }
  else
  {
    fvPatchField<scalar>::operator=(p_);
  }
}

mousse::prghPressureFvPatchScalarField::
prghPressureFvPatchScalarField
(
  const prghPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  rhoName_{ptf.rhoName_},
  p_{ptf.p_, mapper}
{}

mousse::prghPressureFvPatchScalarField::
prghPressureFvPatchScalarField
(
  const prghPressureFvPatchScalarField& ptf
)
:
  fixedValueFvPatchScalarField{ptf},
  rhoName_{ptf.rhoName_},
  p_{ptf.p_}
{}

mousse::prghPressureFvPatchScalarField::
prghPressureFvPatchScalarField
(
  const prghPressureFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{ptf, iF},
  rhoName_{ptf.rhoName_},
  p_{ptf.p_}
{}

// Member Functions 
void mousse::prghPressureFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchScalarField::autoMap(m);
  p_.autoMap(m);
}

void mousse::prghPressureFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchScalarField::rmap(ptf, addr);
  const prghPressureFvPatchScalarField& tiptf =
    refCast<const prghPressureFvPatchScalarField>(ptf);
  p_.rmap(tiptf.p_, addr);
}

void mousse::prghPressureFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const scalarField& rhop = patch().lookupPatchField<volScalarField, scalar>
  (
    rhoName_
  );
  const uniformDimensionedVectorField& g =
    db().lookupObject<uniformDimensionedVectorField>("g");
  const uniformDimensionedScalarField& hRef =
    db().lookupObject<uniformDimensionedScalarField>("hRef");
  dimensionedScalar ghRef
  {
    mag(g.value()) > SMALL
   ? g & (cmptMag(g.value())/mag(g.value()))*hRef
   : dimensionedScalar("ghRef", g.dimensions()*dimLength, 0)
  };
  operator==(p_ - rhop*((g.value() & patch().Cf()) - ghRef.value()));
  fixedValueFvPatchScalarField::updateCoeffs();
}

void mousse::prghPressureFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  p_.writeEntry("p", os);
  writeEntry("value", os);
}

namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  prghPressureFvPatchScalarField
);
}
