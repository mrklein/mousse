// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_velocity_flux_fixed_value_fv_patch_field.hpp"
#include "fv_patch_field_mapper.hpp"
#include "mapped_patch_base.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


// Constructors 
mousse::mappedVelocityFluxFixedValueFvPatchField::
mappedVelocityFluxFixedValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF},
  phiName_{"phi"}
{}


mousse::mappedVelocityFluxFixedValueFvPatchField::
mappedVelocityFluxFixedValueFvPatchField
(
  const mappedVelocityFluxFixedValueFvPatchField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{ptf, p, iF, mapper},
  phiName_{ptf.phiName_}
{
  if (!isA<mappedPatchBase>(this->patch().patch())) {
    FATAL_ERROR_IN
    (
      "mappedVelocityFluxFixedValueFvPatchField::"
      "mappedVelocityFluxFixedValueFvPatchField"
      "("
        "const mappedVelocityFluxFixedValueFvPatchField&, "
        "const fvPatch&, "
        "const DimensionedField<vector, volMesh>&, "
        "const fvPatchFieldMapper&"
      ")"
    )
    << "Patch type '" << p.type()
    << "' not type '" << mappedPatchBase::typeName << "'"
    << " for patch " << p.name()
    << " of field " << dimensionedInternalField().name()
    << " in file " << dimensionedInternalField().objectPath()
    << exit(FatalError);
  }
}


mousse::mappedVelocityFluxFixedValueFvPatchField::
mappedVelocityFluxFixedValueFvPatchField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF, dict},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")}
{
  if (!isA<mappedPatchBase>(this->patch().patch())) {
    FATAL_ERROR_IN
    (
      "mappedVelocityFluxFixedValueFvPatchField::"
      "mappedVelocityFluxFixedValueFvPatchField"
      "("
        "const fvPatch&, "
        "const DimensionedField<vector, volMesh>&, "
        "const dictionary&"
      ")"
    )
    << "Patch type '" << p.type()
    << "' not type '" << mappedPatchBase::typeName << "'"
    << " for patch " << p.name()
    << " of field " << dimensionedInternalField().name()
    << " in file " << dimensionedInternalField().objectPath()
    << exit(FatalError);
  }
  const mappedPatchBase& mpp = refCast<const mappedPatchBase>
  (
    this->patch().patch()
  );
  if (mpp.mode() == mappedPolyPatch::NEARESTCELL) {
    FATAL_ERROR_IN
    (
      "mappedVelocityFluxFixedValueFvPatchField::"
      "mappedVelocityFluxFixedValueFvPatchField"
      "("
        "const fvPatch&, "
        "const DimensionedField<vector, volMesh>&, "
        "const dictionary&"
      ")"
    )
    << "Patch " << p.name()
    << " of type '" << p.type()
    << "' can not be used in 'nearestCell' mode"
    << " of field " << dimensionedInternalField().name()
    << " in file " << dimensionedInternalField().objectPath()
    << exit(FatalError);
  }
}


mousse::mappedVelocityFluxFixedValueFvPatchField::
mappedVelocityFluxFixedValueFvPatchField
(
  const mappedVelocityFluxFixedValueFvPatchField& ptf
)
:
  fixedValueFvPatchVectorField{ptf},
  phiName_{ptf.phiName_}
{}


mousse::mappedVelocityFluxFixedValueFvPatchField::
mappedVelocityFluxFixedValueFvPatchField
(
  const mappedVelocityFluxFixedValueFvPatchField& ptf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{ptf, iF},
  phiName_{ptf.phiName_}
{}


// Member Functions 
void mousse::mappedVelocityFluxFixedValueFvPatchField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  // Since we're inside initEvaluate/evaluate there might be processor
  // comms underway. Change the tag we use.
  int oldTag = UPstream::msgType();
  UPstream::msgType() = oldTag+1;
  // Get the mappedPatchBase
  const mappedPatchBase& mpp = refCast<const mappedPatchBase>
  (
    mappedVelocityFluxFixedValueFvPatchField::patch().patch()
  );
  const fvMesh& nbrMesh = refCast<const fvMesh>(mpp.sampleMesh());
  const word& fieldName = dimensionedInternalField().name();
  const volVectorField& UField =
    nbrMesh.lookupObject<volVectorField>(fieldName);
  surfaceScalarField& phiField = const_cast<surfaceScalarField&>
  (
    nbrMesh.lookupObject<surfaceScalarField>(phiName_)
  );
  vectorField newUValues;
  scalarField newPhiValues;
  switch (mpp.mode()) {
    case mappedPolyPatch::NEARESTFACE:
    {
      vectorField allUValues{nbrMesh.nFaces(), vector::zero};
      scalarField allPhiValues{nbrMesh.nFaces(), 0.0};
      FOR_ALL(UField.boundaryField(), patchI) {
        const fvPatchVectorField& Upf = UField.boundaryField()[patchI];
        const scalarField& phipf = phiField.boundaryField()[patchI];
        label faceStart = Upf.patch().start();
        FOR_ALL(Upf, faceI) {
          allUValues[faceStart + faceI] = Upf[faceI];
          allPhiValues[faceStart + faceI] = phipf[faceI];
        }
      }
      mpp.distribute(allUValues);
      newUValues.transfer(allUValues);
      mpp.distribute(allPhiValues);
      newPhiValues.transfer(allPhiValues);
      break;
    }
    case mappedPolyPatch::NEARESTPATCHFACE:
    case mappedPolyPatch::NEARESTPATCHFACEAMI:
    {
      const label nbrPatchID =
        nbrMesh.boundaryMesh().findPatchID(mpp.samplePatch());
      newUValues = UField.boundaryField()[nbrPatchID];
      mpp.distribute(newUValues);
      newPhiValues = phiField.boundaryField()[nbrPatchID];
      mpp.distribute(newPhiValues);
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "mappedVelocityFluxFixedValueFvPatchField::"
        "updateCoeffs()"
      )
      << "patch can only be used in NEARESTPATCHFACE, "
      << "NEARESTPATCHFACEAMI or NEARESTFACE mode" << nl
      << abort(FatalError);
    }
  }
  operator==(newUValues);
  phiField.boundaryField()[patch().index()] == newPhiValues;
  // Restore tag
  UPstream::msgType() = oldTag;
  fixedValueFvPatchVectorField::updateCoeffs();
}


void mousse::mappedVelocityFluxFixedValueFvPatchField::write
(
  Ostream& os
) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  this->writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  mappedVelocityFluxFixedValueFvPatchField
);

}

