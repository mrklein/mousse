// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "moving_wall_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fvc_mesh_phi.hpp"
#include "time.hpp"

// Constructors 
mousse::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField(p, iF),
  UName_("U")
{}
mousse::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
  const movingWallVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField(ptf, p, iF, mapper),
  UName_(ptf.UName_)
{}
mousse::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField(p, iF),
  UName_(dict.lookupOrDefault<word>("U", "U"))
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
}
mousse::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
  const movingWallVelocityFvPatchVectorField& mwvpvf
)
:
  fixedValueFvPatchVectorField(mwvpvf),
  UName_(mwvpvf.UName_)
{}
mousse::movingWallVelocityFvPatchVectorField::
movingWallVelocityFvPatchVectorField
(
  const movingWallVelocityFvPatchVectorField& mwvpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField(mwvpvf, iF),
  UName_(mwvpvf.UName_)
{}
// Member Functions 
void mousse::movingWallVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const fvMesh& mesh = dimensionedInternalField().mesh();
  if (mesh.moving())
  {
    const fvPatch& p = patch();
    const polyPatch& pp = p.patch();
    const pointField& oldPoints = mesh.oldPoints();
    vectorField oldFc(pp.size());
    FOR_ALL(oldFc, i)
    {
      oldFc[i] = pp[i].centre(oldPoints);
    }
    const scalar deltaT = mesh.time().deltaTValue();
    const vectorField Up((pp.faceCentres() - oldFc)/deltaT);
    const volVectorField& U = db().lookupObject<volVectorField>(UName_);
    scalarField phip
    (
      p.patchField<surfaceScalarField, scalar>(fvc::meshPhi(U))
    );
    const vectorField n(p.nf());
    const scalarField& magSf = p.magSf();
    tmp<scalarField> Un = phip/(magSf + VSMALL);
    vectorField::operator=(Up + n*(Un - (n & Up)));
  }
  fixedValueFvPatchVectorField::updateCoeffs();
}
void mousse::movingWallVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "U", "U", UName_);
  writeEntry("value", os);
}
namespace mousse
{

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  movingWallVelocityFvPatchVectorField
);

}
