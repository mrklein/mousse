// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "outlet_phase_mean_velocity_fv_patch_vector_field.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "surface_fields.hpp"

// Constructors 
mousse::outletPhaseMeanVelocityFvPatchVectorField
::outletPhaseMeanVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  mixedFvPatchField<vector>{p, iF},
  Umean_{0},
  alphaName_{"none"}
{
  refValue() = vector::zero;
  refGrad() = vector::zero;
  valueFraction() = 0.0;
}

mousse::outletPhaseMeanVelocityFvPatchVectorField
::outletPhaseMeanVelocityFvPatchVectorField
(
  const outletPhaseMeanVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchField<vector>{ptf, p, iF, mapper},
  Umean_{ptf.Umean_},
  alphaName_{ptf.alphaName_}
{}

mousse::outletPhaseMeanVelocityFvPatchVectorField
::outletPhaseMeanVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchField<vector>{p, iF},
  Umean_{readScalar(dict.lookup("Umean"))},
  alphaName_{dict.lookup("alpha")}
{
  refValue() = vector::zero;
  refGrad() = vector::zero;
  valueFraction() = 0.0;
  if (dict.found("value"))
  {
    fvPatchVectorField::operator=
    (
      vectorField{"value", dict, p.size()}
    );
  }
  else
  {
    fvPatchVectorField::operator=(patchInternalField());
  }
}

mousse::outletPhaseMeanVelocityFvPatchVectorField
::outletPhaseMeanVelocityFvPatchVectorField
(
  const outletPhaseMeanVelocityFvPatchVectorField& ptf
)
:
  mixedFvPatchField<vector>{ptf},
  Umean_{ptf.Umean_},
  alphaName_{ptf.alphaName_}
{}

mousse::outletPhaseMeanVelocityFvPatchVectorField
::outletPhaseMeanVelocityFvPatchVectorField
(
  const outletPhaseMeanVelocityFvPatchVectorField& ptf,
  const DimensionedField<vector, volMesh>& iF
)
:
  mixedFvPatchField<vector>{ptf, iF},
  Umean_{ptf.Umean_},
  alphaName_{ptf.alphaName_}
{}

// Member Functions 
void mousse::outletPhaseMeanVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  scalarField alphap =
    patch().lookupPatchField<volScalarField, scalar>(alphaName_);
  alphap = max(alphap, scalar(0));
  alphap = min(alphap, scalar(1));
  // Get the patchInternalField (zero-gradient field)
  vectorField Uzg(patchInternalField());
  // Calculate the phase mean zero-gradient velocity
  scalar Uzgmean =
    gSum(alphap*(patch().Sf() & Uzg))
   /gSum(alphap*patch().magSf());
  // Set the refValue and valueFraction to adjust the boundary field
  // such that the phase mean is Umean_
  if (Uzgmean >= Umean_)
  {
    refValue() = vector::zero;
    valueFraction() = 1.0 - Umean_/Uzgmean;
  }
  else
  {
    refValue() = (Umean_ + Uzgmean)*patch().nf();
    valueFraction() = 1.0 - Uzgmean/Umean_;
  }
  mixedFvPatchField<vector>::updateCoeffs();
}

void mousse::outletPhaseMeanVelocityFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchField<vector>::write(os);
  os.writeKeyword("Umean") << Umean_
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
  outletPhaseMeanVelocityFvPatchVectorField
);
}
