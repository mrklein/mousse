// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inlet_outlet_total_temperature_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::inletOutletTotalTemperatureFvPatchScalarField::
inletOutletTotalTemperatureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  inletOutletFvPatchScalarField(p, iF),
  UName_("U"),
  psiName_("psi"),
  gamma_(0.0),
  T0_(p.size(), 0.0)
{
  this->refValue() = pTraits<scalar>::zero;
  this->refGrad() = pTraits<scalar>::zero;
  this->valueFraction() = 0.0;
}
mousse::inletOutletTotalTemperatureFvPatchScalarField::
inletOutletTotalTemperatureFvPatchScalarField
(
  const inletOutletTotalTemperatureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  inletOutletFvPatchScalarField(ptf, p, iF, mapper),
  UName_(ptf.UName_),
  psiName_(ptf.psiName_),
  gamma_(ptf.gamma_),
  T0_(ptf.T0_, mapper)
{}
mousse::inletOutletTotalTemperatureFvPatchScalarField::
inletOutletTotalTemperatureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  inletOutletFvPatchScalarField(p, iF),
  UName_(dict.lookupOrDefault<word>("U", "U")),
  psiName_(dict.lookupOrDefault<word>("psi", "thermo:psi")),
  gamma_(readScalar(dict.lookup("gamma"))),
  T0_("T0", dict, p.size())
{
  this->phiName_ = dict.lookupOrDefault<word>("phi", "phi");
  this->refValue() = pTraits<scalar>::zero;
  if (dict.found("value"))
  {
    fvPatchField<scalar>::operator=
    (
      scalarField("value", dict, p.size())
    );
  }
  else
  {
    fvPatchField<scalar>::operator=(T0_);
  }
  this->refGrad() = pTraits<scalar>::zero;
  this->valueFraction() = 0.0;
}
mousse::inletOutletTotalTemperatureFvPatchScalarField::
inletOutletTotalTemperatureFvPatchScalarField
(
  const inletOutletTotalTemperatureFvPatchScalarField& tppsf
)
:
  inletOutletFvPatchScalarField(tppsf),
  UName_(tppsf.UName_),
  psiName_(tppsf.psiName_),
  gamma_(tppsf.gamma_),
  T0_(tppsf.T0_)
{}
mousse::inletOutletTotalTemperatureFvPatchScalarField::
inletOutletTotalTemperatureFvPatchScalarField
(
  const inletOutletTotalTemperatureFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  inletOutletFvPatchScalarField(tppsf, iF),
  UName_(tppsf.UName_),
  psiName_(tppsf.psiName_),
  gamma_(tppsf.gamma_),
  T0_(tppsf.T0_)
{}
// Member Functions 
void mousse::inletOutletTotalTemperatureFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  inletOutletFvPatchScalarField::autoMap(m);
  T0_.autoMap(m);
}
void mousse::inletOutletTotalTemperatureFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  inletOutletFvPatchScalarField::rmap(ptf, addr);
  const inletOutletTotalTemperatureFvPatchScalarField& tiptf =
    refCast<const inletOutletTotalTemperatureFvPatchScalarField>(ptf);
  T0_.rmap(tiptf.T0_, addr);
}
void mousse::inletOutletTotalTemperatureFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const fvPatchVectorField& Up =
    patch().lookupPatchField<volVectorField, vector>(UName_);
  const fvsPatchField<scalar>& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(this->phiName_);
  const fvPatchField<scalar>& psip =
    patch().lookupPatchField<volScalarField, scalar>(psiName_);
  scalar gM1ByG = (gamma_ - 1.0)/gamma_;
  this->refValue() =
    T0_/(1.0 + 0.5*psip*gM1ByG*(1.0 - pos(phip))*magSqr(Up));
  this->valueFraction() = 1.0 - pos(phip);
  inletOutletFvPatchScalarField::updateCoeffs();
}
void mousse::inletOutletTotalTemperatureFvPatchScalarField::write(Ostream& os)
const
{
  fvPatchScalarField::write(os);
  writeEntryIfDifferent<word>(os, "U", "U", UName_);
  writeEntryIfDifferent<word>(os, "phi", "phi", this->phiName_);
  writeEntryIfDifferent<word>(os, "psi", "psi", psiName_);
  os.writeKeyword("gamma") << gamma_ << token::END_STATEMENT << nl;
  T0_.writeEntry("T0", os);
  writeEntry("value", os);
}

namespace mousse
{

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  inletOutletTotalTemperatureFvPatchScalarField
);

}
