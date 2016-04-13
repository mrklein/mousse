// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "prgh_total_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "uniform_dimensioned_fields.hpp"


// Constructors 
mousse::prghTotalPressureFvPatchScalarField::
prghTotalPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  UName_{"U"},
  phiName_{"phi"},
  rhoName_{"rho"},
  p0_{p.size(), 0.0}
{}


mousse::prghTotalPressureFvPatchScalarField::
prghTotalPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF},
  UName_{dict.lookupOrDefault<word>("U", "U")},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")},
  p0_{"p0", dict, p.size()}
{
  if (dict.found("value")) {
    fvPatchScalarField::operator=
    (
      scalarField{"value", dict, p.size()}
    );
  } else {
    fvPatchField<scalar>::operator=(p0_);
  }
}


mousse::prghTotalPressureFvPatchScalarField::
prghTotalPressureFvPatchScalarField
(
  const prghTotalPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  UName_{ptf.UName_},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  p0_{ptf.p0_, mapper}
{}


mousse::prghTotalPressureFvPatchScalarField::
prghTotalPressureFvPatchScalarField
(
  const prghTotalPressureFvPatchScalarField& ptf
)
:
  fixedValueFvPatchScalarField{ptf},
  UName_{ptf.UName_},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  p0_{ptf.p0_}
{}


mousse::prghTotalPressureFvPatchScalarField::
prghTotalPressureFvPatchScalarField
(
  const prghTotalPressureFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{ptf, iF},
  UName_{ptf.UName_},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  p0_{ptf.p0_}
{}


// Member Functions 
void mousse::prghTotalPressureFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchScalarField::autoMap(m);
  p0_.autoMap(m);
}


void mousse::prghTotalPressureFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchScalarField::rmap(ptf, addr);
  const prghTotalPressureFvPatchScalarField& tiptf =
    refCast<const prghTotalPressureFvPatchScalarField>(ptf);
  p0_.rmap(tiptf.p0_, addr);
}


void mousse::prghTotalPressureFvPatchScalarField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const scalarField& rhop =
    patch().lookupPatchField<volScalarField, scalar>(rhoName_);
  const scalarField& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);
  const vectorField& Up =
    patch().lookupPatchField<volVectorField, vector>(UName_);
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
  operator==
  (
    p0_ - 0.5*rhop*(1.0 - pos(phip))*magSqr(Up)
    - rhop*((g.value() & patch().Cf()) - ghRef.value())
  );
  fixedValueFvPatchScalarField::updateCoeffs();
}


void mousse::prghTotalPressureFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  writeEntryIfDifferent<word>(os, "U", "U", UName_);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  p0_.writeEntry("p0", os);
  writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  prghTotalPressureFvPatchScalarField
);

}

