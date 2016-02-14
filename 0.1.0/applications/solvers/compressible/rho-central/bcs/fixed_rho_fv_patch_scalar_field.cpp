// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_rho_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
// Constructors 
mousse::fixedRhoFvPatchScalarField::fixedRhoFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  pName_{"p"},
  psiName_{"thermo:psi"}
{}
mousse::fixedRhoFvPatchScalarField::fixedRhoFvPatchScalarField
(
  const fixedRhoFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  pName_{ptf.pName_},
  psiName_{ptf.psiName_}
{}
mousse::fixedRhoFvPatchScalarField::fixedRhoFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF, dict},
  pName_{dict.lookupOrDefault<word>("p", "p")},
  psiName_{dict.lookupOrDefault<word>("psi", "thermo:psi")}
{}
mousse::fixedRhoFvPatchScalarField::fixedRhoFvPatchScalarField
(
  const fixedRhoFvPatchScalarField& frpsf
)
:
  fixedValueFvPatchScalarField{frpsf},
  pName_{frpsf.pName_},
  psiName_{frpsf.psiName_}
{}
mousse::fixedRhoFvPatchScalarField::fixedRhoFvPatchScalarField
(
  const fixedRhoFvPatchScalarField& frpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{frpsf, iF},
  pName_{frpsf.pName_},
  psiName_{frpsf.psiName_}
{}
// Member Functions 
void mousse::fixedRhoFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const fvPatchField<scalar>& psip =
    patch().lookupPatchField<volScalarField, scalar>(psiName_);
  const fvPatchField<scalar>& pp =
    patch().lookupPatchField<volScalarField, scalar>(pName_);
  operator==(psip*pp);
  fixedValueFvPatchScalarField::updateCoeffs();
}
void mousse::fixedRhoFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  writeEntryIfDifferent<word>(os, "p", "p", this->pName_);
  writeEntryIfDifferent<word>(os, "psi", "thermo:psi", psiName_);
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  fixedRhoFvPatchScalarField
);
}
