// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "variable_height_flow_rate_fv_patch_field.hpp"
#include "fv_patch_field_mapper.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::variableHeightFlowRateFvPatchScalarField
::variableHeightFlowRateFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchField<scalar>(p, iF),
  phiName_("phi"),
  lowerBound_(0.0),
  upperBound_(1.0)
{
  this->refValue() = 0.0;
  this->refGrad() = 0.0;
  this->valueFraction() = 0.0;
}
mousse::variableHeightFlowRateFvPatchScalarField
::variableHeightFlowRateFvPatchScalarField
(
  const variableHeightFlowRateFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField(ptf, p, iF, mapper),
  phiName_(ptf.phiName_),
  lowerBound_(ptf.lowerBound_),
  upperBound_(ptf.upperBound_)
{}
mousse::variableHeightFlowRateFvPatchScalarField
::variableHeightFlowRateFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField(p, iF),
  phiName_(dict.lookupOrDefault<word>("phi", "phi")),
  lowerBound_(readScalar(dict.lookup("lowerBound"))),
  upperBound_(readScalar(dict.lookup("upperBound")))
{
  this->refValue() = 0.0;
  if (dict.found("value"))
  {
    fvPatchScalarField::operator=
    (
      scalarField("value", dict, p.size())
    );
  }
  else
  {
    fvPatchScalarField::operator=(this->patchInternalField());
  }
  this->refGrad() = 0.0;
  this->valueFraction() = 0.0;
}
mousse::variableHeightFlowRateFvPatchScalarField
  ::variableHeightFlowRateFvPatchScalarField
(
  const variableHeightFlowRateFvPatchScalarField& ptf
)
:
  mixedFvPatchScalarField(ptf),
  phiName_(ptf.phiName_),
  lowerBound_(ptf.lowerBound_),
  upperBound_(ptf.upperBound_)
{}
mousse::variableHeightFlowRateFvPatchScalarField
  ::variableHeightFlowRateFvPatchScalarField
(
  const variableHeightFlowRateFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField(ptf, iF),
  phiName_(ptf.phiName_),
  lowerBound_(ptf.lowerBound_),
  upperBound_(ptf.upperBound_)
{}
// Member Functions 
void mousse::variableHeightFlowRateFvPatchScalarField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const fvsPatchField<scalar>& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);
  scalarField alphap(this->patchInternalField());
  FOR_ALL(phip, i)
  {
    if (phip[i] < -SMALL)
    {
      if (alphap[i] < lowerBound_)
      {
        this->refValue()[i] = 0.0;
      }
      else if (alphap[i] > upperBound_)
      {
        this->refValue()[i] = 1.0;
      }
      else
      {
        this->refValue()[i] = alphap[i];
      }
      this->valueFraction()[i] = 1.0;
    }
    else
    {
      this->refValue()[i] = 0.0;
      this->valueFraction()[i] = 0.0;
    }
  }
  mixedFvPatchScalarField::updateCoeffs();
}
void mousse::variableHeightFlowRateFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  if (phiName_ != "phi")
  {
    os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
  }
  os.writeKeyword("lowerBound") << lowerBound_ << token::END_STATEMENT << nl;
  os.writeKeyword("upperBound") << upperBound_ << token::END_STATEMENT << nl;
  this->writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  variableHeightFlowRateFvPatchScalarField
);
}
