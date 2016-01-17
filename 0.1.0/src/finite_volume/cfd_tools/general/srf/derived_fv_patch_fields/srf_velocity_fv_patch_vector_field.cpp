// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "srf_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "srf_model.hpp"
// Constructors
mousse::SRFVelocityFvPatchVectorField::SRFVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF},
  relative_{0},
  inletValue_{p.size(), vector::zero}
{}
mousse::SRFVelocityFvPatchVectorField::SRFVelocityFvPatchVectorField
(
  const SRFVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{ptf, p, iF, mapper},
  relative_{ptf.relative_},
  inletValue_{ptf.inletValue_, mapper}
{}
mousse::SRFVelocityFvPatchVectorField::SRFVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF},
  relative_{dict.lookup("relative")},
  inletValue_{"inletValue", dict, p.size()}
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
}
mousse::SRFVelocityFvPatchVectorField::SRFVelocityFvPatchVectorField
(
  const SRFVelocityFvPatchVectorField& srfvpvf
)
:
  fixedValueFvPatchVectorField{srfvpvf},
  relative_{srfvpvf.relative_},
  inletValue_{srfvpvf.inletValue_}
{}
mousse::SRFVelocityFvPatchVectorField::SRFVelocityFvPatchVectorField
(
  const SRFVelocityFvPatchVectorField& srfvpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{srfvpvf, iF},
  relative_{srfvpvf.relative_},
  inletValue_{srfvpvf.inletValue_}
{}
// Member Functions
void mousse::SRFVelocityFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  vectorField::autoMap(m);
  inletValue_.autoMap(m);
}
void mousse::SRFVelocityFvPatchVectorField::rmap
(
  const fvPatchVectorField& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchVectorField::rmap(ptf, addr);
  const SRFVelocityFvPatchVectorField& tiptf =
    refCast<const SRFVelocityFvPatchVectorField>(ptf);
  inletValue_.rmap(tiptf.inletValue_, addr);
}
void mousse::SRFVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  // If not relative to the SRF include the effect of the SRF
  if (!relative_)
  {
    // Get reference to the SRF model
    const SRF::SRFModel& srf =
      db().lookupObject<SRF::SRFModel>("SRFProperties");
    // Determine patch velocity due to SRF
    const vectorField SRFVelocity(srf.velocity(patch().Cf()));
    operator==(-SRFVelocity + inletValue_);
  }
  // If already relative to the SRF simply supply the inlet value as a fixed
  // value
  else
  {
    operator==(inletValue_);
  }
  fixedValueFvPatchVectorField::updateCoeffs();
}
void mousse::SRFVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  os.writeKeyword("relative") << relative_ << token::END_STATEMENT << nl;
  inletValue_.writeEntry("inletValue", os);
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  SRFVelocityFvPatchVectorField
);
}
