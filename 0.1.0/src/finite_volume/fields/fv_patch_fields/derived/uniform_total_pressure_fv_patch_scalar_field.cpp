// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_total_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::uniformTotalPressureFvPatchScalarField::
uniformTotalPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(p, iF),
  UName_("U"),
  phiName_("phi"),
  rhoName_("none"),
  psiName_("none"),
  gamma_(0.0),
  pressure_()
{}
mousse::uniformTotalPressureFvPatchScalarField::
uniformTotalPressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField(p, iF),
  UName_(dict.lookupOrDefault<word>("U", "U")),
  phiName_(dict.lookupOrDefault<word>("phi", "phi")),
  rhoName_(dict.lookupOrDefault<word>("rho", "none")),
  psiName_(dict.lookupOrDefault<word>("psi", "none")),
  gamma_(readScalar(dict.lookup("gamma"))),
  pressure_(DataEntry<scalar>::New("pressure", dict))
{
  if (dict.found("value"))
  {
    fvPatchField<scalar>::operator=
    (
      scalarField("value", dict, p.size())
    );
  }
  else
  {
    scalar p0 = pressure_->value(this->db().time().timeOutputValue());
    fvPatchField<scalar>::operator=(p0);
  }
}
mousse::uniformTotalPressureFvPatchScalarField::
uniformTotalPressureFvPatchScalarField
(
  const uniformTotalPressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField(p, iF),  // bypass mapper
  UName_(ptf.UName_),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_),
  psiName_(ptf.psiName_),
  gamma_(ptf.gamma_),
  pressure_(ptf.pressure_().clone().ptr())
{
  // Evaluate since value not mapped
  const scalar t = this->db().time().timeOutputValue();
  fvPatchScalarField::operator==(pressure_->value(t));
}
mousse::uniformTotalPressureFvPatchScalarField::
uniformTotalPressureFvPatchScalarField
(
  const uniformTotalPressureFvPatchScalarField& ptf
)
:
  fixedValueFvPatchScalarField(ptf),
  UName_(ptf.UName_),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_),
  psiName_(ptf.psiName_),
  gamma_(ptf.gamma_),
  pressure_
  (
    ptf.pressure_.valid()
   ? ptf.pressure_().clone().ptr()
   : NULL
  )
{}
mousse::uniformTotalPressureFvPatchScalarField::
uniformTotalPressureFvPatchScalarField
(
  const uniformTotalPressureFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(ptf, iF),
  UName_(ptf.UName_),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_),
  psiName_(ptf.psiName_),
  gamma_(ptf.gamma_),
  pressure_
  (
    ptf.pressure_.valid()
   ? ptf.pressure_().clone().ptr()
   : NULL
  )
{}
// Member Functions 
void mousse::uniformTotalPressureFvPatchScalarField::updateCoeffs
(
  const vectorField& Up
)
{
  if (updated())
  {
    return;
  }
  scalar p0 = pressure_->value(this->db().time().timeOutputValue());
  const fvsPatchField<scalar>& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);
  if (psiName_ == "none" && rhoName_ == "none")
  {
    operator==(p0 - 0.5*(1.0 - pos(phip))*magSqr(Up));
  }
  else if (rhoName_ == "none")
  {
    const fvPatchField<scalar>& psip =
      patch().lookupPatchField<volScalarField, scalar>(psiName_);
    if (gamma_ > 1.0)
    {
      scalar gM1ByG = (gamma_ - 1.0)/gamma_;
      operator==
      (
        p0
       /pow
        (
          (1.0 + 0.5*psip*gM1ByG*(1.0 - pos(phip))*magSqr(Up)),
          1.0/gM1ByG
        )
      );
    }
    else
    {
      operator==(p0/(1.0 + 0.5*psip*(1.0 - pos(phip))*magSqr(Up)));
    }
  }
  else if (psiName_ == "none")
  {
    const fvPatchField<scalar>& rho =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    operator==(p0 - 0.5*rho*(1.0 - pos(phip))*magSqr(Up));
  }
  else
  {
    FatalErrorIn("uniformTotalPressureFvPatchScalarField::updateCoeffs()")
      << " rho or psi set inconsitently, rho = " << rhoName_
      << ", psi = " << psiName_ << ".\n"
      << "    Set either rho or psi or neither depending on the "
       "definition of total pressure.\n"
      << "    Set the unused variables to 'none'.\n"
      << "    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalError);
  }
  fixedValueFvPatchScalarField::updateCoeffs();
}
void mousse::uniformTotalPressureFvPatchScalarField::updateCoeffs()
{
  updateCoeffs(patch().lookupPatchField<volVectorField, vector>(UName_));
}
void mousse::uniformTotalPressureFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  writeEntryIfDifferent<word>(os, "U", "U", UName_);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  os.writeKeyword("rho") << rhoName_ << token::END_STATEMENT << nl;
  os.writeKeyword("psi") << psiName_ << token::END_STATEMENT << nl;
  os.writeKeyword("gamma") << gamma_ << token::END_STATEMENT << nl;
  pressure_->writeData(os);
  writeEntry("value", os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    uniformTotalPressureFvPatchScalarField
  );
}
