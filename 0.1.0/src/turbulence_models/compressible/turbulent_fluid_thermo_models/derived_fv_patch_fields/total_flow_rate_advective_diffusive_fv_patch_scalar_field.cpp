// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "total_flow_rate_advective_diffusive_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "ioobject_list.hpp"
#include "turbulent_fluid_thermo_model.hpp"
// Constructors 
mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchField<scalar>{p, iF},
  phiName_{"phi"},
  rhoName_{"none"},
  massFluxFraction_{1.0}
{
  refValue() = 0.0;
  refGrad() = 0.0;
  valueFraction() = 0.0;
}
mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchField<scalar>{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "none")},
  massFluxFraction_{dict.lookupOrDefault<scalar>("massFluxFraction", 1.0)}
{
  refValue() = 1.0;
  refGrad() = 0.0;
  valueFraction() = 0.0;
  if (dict.found("value"))
  {
    fvPatchField<scalar>::operator=
    (
      Field<scalar>("value", dict, p.size())
    );
  }
  else
  {
    fvPatchField<scalar>::operator=(refValue());
  }
}
mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
  const totalFlowRateAdvectiveDiffusiveFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchField<scalar>{ptf, p, iF, mapper},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  massFluxFraction_{ptf.massFluxFraction_}
{}
mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
  const totalFlowRateAdvectiveDiffusiveFvPatchScalarField& tppsf
)
:
  mixedFvPatchField<scalar>{tppsf},
  phiName_{tppsf.phiName_},
  rhoName_{tppsf.rhoName_},
  massFluxFraction_{tppsf.massFluxFraction_}
{}
mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
totalFlowRateAdvectiveDiffusiveFvPatchScalarField
(
  const totalFlowRateAdvectiveDiffusiveFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchField<scalar>{tppsf, iF},
  phiName_{tppsf.phiName_},
  rhoName_{tppsf.rhoName_},
  massFluxFraction_{tppsf.massFluxFraction_}
{}
// Member Functions 
void mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  scalarField::autoMap(m);
}
void mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  mixedFvPatchField<scalar>::rmap(ptf, addr);
}
void mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const label patchI = patch().index();
  const LESModel<EddyDiffusivity<compressible::turbulenceModel> >& turbModel =
    db().lookupObject
    <
      LESModel<EddyDiffusivity<compressible::turbulenceModel> >
    >
    (
      IOobject::groupName
      (
        turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const fvsPatchField<scalar>& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);
  const scalarField alphap(turbModel.alphaEff(patchI));
  refValue() = massFluxFraction_;
  refGrad() = 0.0;
  valueFraction() =
    1.0
    /
    (
      1.0 +
      alphap*patch().deltaCoeffs()*patch().magSf()/max(mag(phip), SMALL)
    );
  mixedFvPatchField<scalar>::updateCoeffs();
  if (debug)
  {
    scalar phi = gSum(-phip*(*this));
    Info<< patch().boundaryMesh().mesh().name() << ':'
      << patch().name() << ':'
      << this->dimensionedInternalField().name() << " :"
      << " mass flux[Kg/s]:" << phi
      << endl;
  }
}
void mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField::
write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
  os.writeKeyword("rho") << rhoName_ << token::END_STATEMENT << nl;
  os.writeKeyword("massFluxFraction") << massFluxFraction_
    << token::END_STATEMENT << nl;
  this->writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  totalFlowRateAdvectiveDiffusiveFvPatchScalarField
);
}
