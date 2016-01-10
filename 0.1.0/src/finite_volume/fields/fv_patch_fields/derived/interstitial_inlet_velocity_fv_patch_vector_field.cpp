// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interstitial_inlet_velocity_fv_patch_vector_field.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "surface_fields.hpp"

// Constructors 
mousse::interstitialInletVelocityFvPatchVectorField::
interstitialInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF},
  inletVelocity_{p.size(), vector::zero},
  alphaName_{"alpha"}
{}

mousse::interstitialInletVelocityFvPatchVectorField::
interstitialInletVelocityFvPatchVectorField
(
  const interstitialInletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{ptf, p, iF, mapper},
  inletVelocity_{ptf.inletVelocity_, mapper},
  alphaName_{ptf.alphaName_}
{}

mousse::interstitialInletVelocityFvPatchVectorField::
interstitialInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF, dict},
  inletVelocity_{"inletVelocity", dict, p.size()},
  alphaName_{dict.lookupOrDefault<word>("alpha", "alpha")}
{}

mousse::interstitialInletVelocityFvPatchVectorField::
interstitialInletVelocityFvPatchVectorField
(
  const interstitialInletVelocityFvPatchVectorField& ptf
)
:
  fixedValueFvPatchVectorField{ptf},
  inletVelocity_{ptf.inletVelocity_},
  alphaName_{ptf.alphaName_}
{}

mousse::interstitialInletVelocityFvPatchVectorField::
interstitialInletVelocityFvPatchVectorField
(
  const interstitialInletVelocityFvPatchVectorField& ptf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{ptf, iF},
  inletVelocity_{ptf.inletVelocity_},
  alphaName_{ptf.alphaName_}
{}

// Member Functions 
void mousse::interstitialInletVelocityFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchVectorField::autoMap(m);
  inletVelocity_.autoMap(m);
}

void mousse::interstitialInletVelocityFvPatchVectorField::rmap
(
  const fvPatchVectorField& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchVectorField::rmap(ptf, addr);
  const interstitialInletVelocityFvPatchVectorField& tiptf =
    refCast<const interstitialInletVelocityFvPatchVectorField>(ptf);
  inletVelocity_.rmap(tiptf.inletVelocity_, addr);
}

void mousse::interstitialInletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const fvPatchField<scalar>& alphap =
    patch().lookupPatchField<volScalarField, scalar>(alphaName_);
  operator==(inletVelocity_/alphap);
  fixedValueFvPatchVectorField::updateCoeffs();
}

void mousse::interstitialInletVelocityFvPatchVectorField::write(Ostream& os)
  const
{
  fvPatchField<vector>::write(os);
  writeEntryIfDifferent<word>(os, "alpha", "alpha", alphaName_);
  inletVelocity_.writeEntry("inletVelocity", os);
  writeEntry("value", os);
}

namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  interstitialInletVelocityFvPatchVectorField
);
}
