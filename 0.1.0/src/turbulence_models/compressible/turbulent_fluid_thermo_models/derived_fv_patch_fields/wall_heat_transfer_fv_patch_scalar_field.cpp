// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_heat_transfer_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "turbulent_fluid_thermo_model.hpp"


// Constructors 
mousse::wallHeatTransferFvPatchScalarField::wallHeatTransferFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{p, iF},
  Tinf_{p.size(), 0.0},
  alphaWall_{p.size(), 0.0}
{
  refValue() = 0.0;
  refGrad() = 0.0;
  valueFraction() = 0.0;
}


mousse::wallHeatTransferFvPatchScalarField::wallHeatTransferFvPatchScalarField
(
  const wallHeatTransferFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField{ptf, p, iF, mapper},
  Tinf_{ptf.Tinf_, mapper},
  alphaWall_{ptf.alphaWall_, mapper}
{}


mousse::wallHeatTransferFvPatchScalarField::wallHeatTransferFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField{p, iF},
  Tinf_{"Tinf", dict, p.size()},
  alphaWall_{"alphaWall", dict, p.size()}
{
  refValue() = Tinf_;
  refGrad() = 0.0;
  valueFraction() = 0.0;
  if (dict.found("value")) {
    fvPatchField<scalar>::operator=
    (
      scalarField{"value", dict, p.size()}
    );
  } else {
    evaluate();
  }
}


mousse::wallHeatTransferFvPatchScalarField::wallHeatTransferFvPatchScalarField
(
  const wallHeatTransferFvPatchScalarField& tppsf
)
:
  mixedFvPatchScalarField{tppsf},
  Tinf_{tppsf.Tinf_},
  alphaWall_{tppsf.alphaWall_}
{}


mousse::wallHeatTransferFvPatchScalarField::wallHeatTransferFvPatchScalarField
(
  const wallHeatTransferFvPatchScalarField& tppsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{tppsf, iF},
  Tinf_{tppsf.Tinf_},
  alphaWall_{tppsf.alphaWall_}
{}


// Member Functions 
void mousse::wallHeatTransferFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  scalarField::autoMap(m);
  Tinf_.autoMap(m);
  alphaWall_.autoMap(m);
}


void mousse::wallHeatTransferFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  mixedFvPatchScalarField::rmap(ptf, addr);
  const wallHeatTransferFvPatchScalarField& tiptf =
    refCast<const wallHeatTransferFvPatchScalarField>(ptf);
  Tinf_.rmap(tiptf.Tinf_, addr);
  alphaWall_.rmap(tiptf.alphaWall_, addr);
}


void mousse::wallHeatTransferFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const compressible::turbulenceModel& turbModel =
    db().lookupObject<compressible::turbulenceModel>
    (
      IOobject::groupName
      (
        turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const label patchi = patch().index();
  valueFraction() =
    1.0/
    (
      1.0 + turbModel.kappaEff(patchi)*patch().deltaCoeffs()/alphaWall_
    );
  mixedFvPatchScalarField::updateCoeffs();
}


void mousse::wallHeatTransferFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  Tinf_.writeEntry("Tinf", os);
  alphaWall_.writeEntry("alphaWall", os);
  writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  wallHeatTransferFvPatchScalarField
);

}

