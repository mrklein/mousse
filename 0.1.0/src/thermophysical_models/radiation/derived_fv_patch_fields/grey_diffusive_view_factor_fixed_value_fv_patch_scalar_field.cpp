// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "grey_diffusive_view_factor_fixed_value_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"


// Constructors 
mousse::radiation::greyDiffusiveViewFactorFixedValueFvPatchScalarField::
greyDiffusiveViewFactorFixedValueFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  radiationCoupledBase{patch(), "undefined", scalarField::null()},
  Qro_{p.size(), 0.0}
{}


mousse::radiation::greyDiffusiveViewFactorFixedValueFvPatchScalarField::
greyDiffusiveViewFactorFixedValueFvPatchScalarField
(
  const greyDiffusiveViewFactorFixedValueFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  radiationCoupledBase{patch(), ptf.emissivityMethod(), ptf.emissivity_},
  Qro_{ptf.Qro_}
{}


mousse::radiation::greyDiffusiveViewFactorFixedValueFvPatchScalarField::
greyDiffusiveViewFactorFixedValueFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF},
  radiationCoupledBase{p, dict},
  Qro_{"Qro", dict, p.size()}
{
  if (dict.found("value")) {
    fvPatchScalarField::operator=(scalarField{"value", dict, p.size()});
  } else {
    fvPatchScalarField::operator=(0.0);
  }
}


mousse::radiation::greyDiffusiveViewFactorFixedValueFvPatchScalarField::
greyDiffusiveViewFactorFixedValueFvPatchScalarField
(
  const greyDiffusiveViewFactorFixedValueFvPatchScalarField& ptf
)
:
  fixedValueFvPatchScalarField{ptf},
  radiationCoupledBase{ptf.patch(), ptf.emissivityMethod(), ptf.emissivity_},
  Qro_{ptf.Qro_}
{}


mousse::radiation::greyDiffusiveViewFactorFixedValueFvPatchScalarField::
greyDiffusiveViewFactorFixedValueFvPatchScalarField
(
  const greyDiffusiveViewFactorFixedValueFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{ptf, iF},
  radiationCoupledBase{ptf.patch(), ptf.emissivityMethod(), ptf.emissivity_},
  Qro_{ptf.Qro_}
{}


// Member Functions 
void mousse::radiation::greyDiffusiveViewFactorFixedValueFvPatchScalarField::
updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  // Do nothing
  if (debug) {
    scalar Q = gSum((*this)*patch().magSf());
    Info << patch().boundaryMesh().mesh().name() << ':'
      << patch().name() << ':'
      << this->dimensionedInternalField().name() << " <- "
      << " heat transfer rate:" << Q
      << " wall radiative heat flux "
      << " min:" << gMin(*this)
      << " max:" << gMax(*this)
      << " avg:" << gAverage(*this)
      << endl;
  }
  fixedValueFvPatchScalarField::updateCoeffs();
}


void mousse::radiation::greyDiffusiveViewFactorFixedValueFvPatchScalarField::
write
(
  Ostream& os
) const
{
  fixedValueFvPatchScalarField::write(os);
  radiationCoupledBase::write(os);
  Qro_.writeEntry("Qro", os);
}


namespace mousse {
namespace radiation {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  greyDiffusiveViewFactorFixedValueFvPatchScalarField
);

}
}

