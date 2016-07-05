// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "alphat_phase_change_wall_function_fv_patch_scalar_field.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace compressible {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(alphatPhaseChangeWallFunctionFvPatchScalarField,0);


// Constructors 
alphatPhaseChangeWallFunctionFvPatchScalarField::
alphatPhaseChangeWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  dmdt_{p.size(), 0.0}
{}


alphatPhaseChangeWallFunctionFvPatchScalarField::
alphatPhaseChangeWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF, dict},
  dmdt_{p.size(), 0.0}
{
  if (dict.found("dmdt")) {
    dmdt_ = scalarField("dmdt", dict, p.size());
  }
}


alphatPhaseChangeWallFunctionFvPatchScalarField::
alphatPhaseChangeWallFunctionFvPatchScalarField
(
  const alphatPhaseChangeWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  dmdt_{ptf.dmdt_, mapper}
{}


alphatPhaseChangeWallFunctionFvPatchScalarField::
alphatPhaseChangeWallFunctionFvPatchScalarField
(
  const alphatPhaseChangeWallFunctionFvPatchScalarField& awfpsf
)
:
  fixedValueFvPatchScalarField{awfpsf},
  dmdt_{awfpsf.dmdt_}
{}


alphatPhaseChangeWallFunctionFvPatchScalarField::
alphatPhaseChangeWallFunctionFvPatchScalarField
(
  const alphatPhaseChangeWallFunctionFvPatchScalarField& awfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{awfpsf, iF},
  dmdt_{awfpsf.dmdt_}
{}


// Member Functions 
void alphatPhaseChangeWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  dmdt_.writeEntry("dmdt", os);
  writeEntry("value", os);
}

}  // namespace compressible
}  // namespace mousse

