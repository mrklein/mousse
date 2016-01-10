// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pressure_directed_inlet_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::pressureDirectedInletVelocityFvPatchVectorField::
pressureDirectedInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF},
  phiName_{"phi"},
  rhoName_{"rho"},
  inletDir_{p.size()}
{}
mousse::pressureDirectedInletVelocityFvPatchVectorField::
pressureDirectedInletVelocityFvPatchVectorField
(
  const pressureDirectedInletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{ptf, p, iF, mapper},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  inletDir_{ptf.inletDir_, mapper}
{}
mousse::pressureDirectedInletVelocityFvPatchVectorField::
pressureDirectedInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")},
  inletDir_{"inletDirection", dict, p.size()}
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
}
mousse::pressureDirectedInletVelocityFvPatchVectorField::
pressureDirectedInletVelocityFvPatchVectorField
(
  const pressureDirectedInletVelocityFvPatchVectorField& pivpvf
)
:
  fixedValueFvPatchVectorField{pivpvf},
  phiName_{pivpvf.phiName_},
  rhoName_{pivpvf.rhoName_},
  inletDir_{pivpvf.inletDir_}
{}
mousse::pressureDirectedInletVelocityFvPatchVectorField::
pressureDirectedInletVelocityFvPatchVectorField
(
  const pressureDirectedInletVelocityFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{pivpvf, iF},
  phiName_{pivpvf.phiName_},
  rhoName_{pivpvf.rhoName_},
  inletDir_{pivpvf.inletDir_}
{}
// Member Functions 
void mousse::pressureDirectedInletVelocityFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchVectorField::autoMap(m);
  inletDir_.autoMap(m);
}
void mousse::pressureDirectedInletVelocityFvPatchVectorField::rmap
(
  const fvPatchVectorField& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchVectorField::rmap(ptf, addr);
  const pressureDirectedInletVelocityFvPatchVectorField& tiptf =
    refCast<const pressureDirectedInletVelocityFvPatchVectorField>(ptf);
  inletDir_.rmap(tiptf.inletDir_, addr);
}
void mousse::pressureDirectedInletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  const fvsPatchField<scalar>& phip =
    patch().patchField<surfaceScalarField, scalar>(phi);
  tmp<vectorField> n = patch().nf();
  tmp<scalarField> ndmagS = (n & inletDir_)*patch().magSf();
  if (phi.dimensions() == dimVelocity*dimArea)
  {
    operator==(inletDir_*phip/ndmagS);
  }
  else if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
  {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    operator==(inletDir_*phip/(rhop*ndmagS));
  }
  else
  {
    FATAL_ERROR_IN
    (
      "pressureDirectedInletVelocityFvPatchVectorField::updateCoeffs()"
    )   << "dimensions of phi are not correct"
      << "\n    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalError);
  }
  fixedValueFvPatchVectorField::updateCoeffs();
}
void mousse::pressureDirectedInletVelocityFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  inletDir_.writeEntry("inletDirection", os);
  writeEntry("value", os);
}
// Member Operators 
void mousse::pressureDirectedInletVelocityFvPatchVectorField::operator=
(
  const fvPatchField<vector>& pvf
)
{
  fvPatchField<vector>::operator=(inletDir_*(inletDir_ & pvf));
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  pressureDirectedInletVelocityFvPatchVectorField
);
}
