// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "total_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


// Constructors 
mousse::totalPressureFvPatchScalarField::totalPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  UName_{"U"},
  phiName_{"phi"},
  rhoName_{"none"},
  psiName_{"none"},
  gamma_{0.0},
  p0_{p.size(), 0.0}
{}


mousse::totalPressureFvPatchScalarField::totalPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF},
  UName_{dict.lookupOrDefault<word>("U", "U")},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "none")},
  psiName_{dict.lookupOrDefault<word>("psi", "none")},
  gamma_{readScalar(dict.lookup("gamma"))},
  p0_{"p0", dict, p.size()}
{
  if (dict.found("value")) {
    fvPatchField<scalar>::operator=
    (
      scalarField{"value", dict, p.size()}
    );
  } else {
    fvPatchField<scalar>::operator=(p0_);
  }
}


mousse::totalPressureFvPatchScalarField::totalPressureFvPatchScalarField
(
  const totalPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  UName_{ptf.UName_},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  psiName_{ptf.psiName_},
  gamma_{ptf.gamma_},
  p0_{ptf.p0_, mapper}
{}


mousse::totalPressureFvPatchScalarField::totalPressureFvPatchScalarField
(
  const totalPressureFvPatchScalarField& tppsf
)
:
  fixedValueFvPatchScalarField{tppsf},
  UName_{tppsf.UName_},
  phiName_{tppsf.phiName_},
  rhoName_{tppsf.rhoName_},
  psiName_{tppsf.psiName_},
  gamma_{tppsf.gamma_},
  p0_{tppsf.p0_}
{}


mousse::totalPressureFvPatchScalarField::totalPressureFvPatchScalarField
(
  const totalPressureFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{tppsf, iF},
  UName_{tppsf.UName_},
  phiName_{tppsf.phiName_},
  rhoName_{tppsf.rhoName_},
  psiName_{tppsf.psiName_},
  gamma_{tppsf.gamma_},
  p0_{tppsf.p0_}
{}


// Member Functions 
void mousse::totalPressureFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchScalarField::autoMap(m);
  p0_.autoMap(m);
}


void mousse::totalPressureFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchScalarField::rmap(ptf, addr);
  const totalPressureFvPatchScalarField& tiptf =
    refCast<const totalPressureFvPatchScalarField>(ptf);
  p0_.rmap(tiptf.p0_, addr);
}


void mousse::totalPressureFvPatchScalarField::updateCoeffs
(
  const scalarField& p0p,
  const vectorField& Up
)
{
  if (updated()) {
    return;
  }
  const fvsPatchField<scalar>& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);
  if (psiName_ == "none" && rhoName_ == "none") {
    operator==(p0p - 0.5*(1.0 - pos(phip))*magSqr(Up));
  } else if (rhoName_ == "none") {
    const fvPatchField<scalar>& psip =
      patch().lookupPatchField<volScalarField, scalar>(psiName_);
    if (gamma_ > 1.0) {
      scalar gM1ByG = (gamma_ - 1.0)/gamma_;
      operator==
      (
        p0p/pow((1.0 + 0.5*psip*gM1ByG*(1.0 - pos(phip))*magSqr(Up)),
                1.0/gM1ByG));
    } else {
      operator==(p0p/(1.0 + 0.5*psip*(1.0 - pos(phip))*magSqr(Up)));
    }
  } else if (psiName_ == "none") {
    const fvPatchField<scalar>& rho =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    operator==(p0p - 0.5*rho*(1.0 - pos(phip))*magSqr(Up));
  } else {
    FATAL_ERROR_IN
    (
      "totalPressureFvPatchScalarField::updateCoeffs()"
    )
    << " rho or psi set inconsistently, rho = " << rhoName_
    << ", psi = " << psiName_ << ".\n"
    << "    Set either rho or psi or neither depending on the "
       "definition of total pressure." << nl
    << "    Set the unused variable(s) to 'none'.\n"
    << "    on patch " << this->patch().name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalError);
  }
  fixedValueFvPatchScalarField::updateCoeffs();
}


void mousse::totalPressureFvPatchScalarField::updateCoeffs()
{
  updateCoeffs
  (
    p0(),
    patch().lookupPatchField<volVectorField, vector>(UName())
  );
}


void mousse::totalPressureFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  writeEntryIfDifferent<word>(os, "U", "U", UName_);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  os.writeKeyword("rho") << rhoName_ << token::END_STATEMENT << nl;
  os.writeKeyword("psi") << psiName_ << token::END_STATEMENT << nl;
  os.writeKeyword("gamma") << gamma_ << token::END_STATEMENT << nl;
  p0_.writeEntry("p0", os);
  writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  totalPressureFvPatchScalarField
);

}
