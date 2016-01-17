// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "variable_height_flow_rate_inlet_velocity_fv_patch_vector_field.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "surface_fields.hpp"

// Constructors 
mousse::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>{p, iF},
  flowRate_{0},
  alphaName_{"none"}
{}

mousse::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
  const variableHeightFlowRateInletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<vector>{ptf, p, iF, mapper},
  flowRate_{ptf.flowRate_},
  alphaName_{ptf.alphaName_}
{}

mousse::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<vector>{p, iF, dict},
  flowRate_{readScalar(dict.lookup("flowRate"))},
  alphaName_{dict.lookup("alpha")}
{}

mousse::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
  const variableHeightFlowRateInletVelocityFvPatchVectorField& ptf
)
:
  fixedValueFvPatchField<vector>{ptf},
  flowRate_{ptf.flowRate_},
  alphaName_{ptf.alphaName_}
{}

mousse::variableHeightFlowRateInletVelocityFvPatchVectorField
::variableHeightFlowRateInletVelocityFvPatchVectorField
(
  const variableHeightFlowRateInletVelocityFvPatchVectorField& ptf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>{ptf, iF},
  flowRate_{ptf.flowRate_},
  alphaName_{ptf.alphaName_}
{}

// Member Functions 
void mousse::variableHeightFlowRateInletVelocityFvPatchVectorField
::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  scalarField alphap =
    patch().lookupPatchField<volScalarField, scalar>(alphaName_);
  alphap = max(alphap, scalar(0));
  alphap = min(alphap, scalar(1));
  // a simpler way of doing this would be nice
  scalar avgU = -flowRate_/gSum(patch().magSf()*alphap);
  vectorField n(patch().nf());
  operator==(n*avgU*alphap);
  fixedValueFvPatchField<vector>::updateCoeffs();
}

void mousse::variableHeightFlowRateInletVelocityFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchField<vector>::write(os);
  os.writeKeyword("flowRate") << flowRate_
    << token::END_STATEMENT << nl;
  os.writeKeyword("alpha") << alphaName_
    << token::END_STATEMENT << nl;
  writeEntry("value", os);
}

namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  variableHeightFlowRateInletVelocityFvPatchVectorField
);
}
