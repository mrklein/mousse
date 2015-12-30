// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_flow_rate_fv_patch_vector_field.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "mapped_patch_base.hpp"
#include "surface_fields.hpp"
// Constructors 
mousse::mappedFlowRateFvPatchVectorField::mappedFlowRateFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>(p, iF),
  nbrPhiName_("none"),
  phiName_("phi"),
  rhoName_("rho")
{}
mousse::mappedFlowRateFvPatchVectorField::mappedFlowRateFvPatchVectorField
(
  const mappedFlowRateFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<vector>(ptf, p, iF, mapper),
  nbrPhiName_(ptf.nbrPhiName_),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_)
{}
mousse::mappedFlowRateFvPatchVectorField::mappedFlowRateFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<vector>(p, iF, dict),
  nbrPhiName_(dict.lookupOrDefault<word>("nbrPhi", "phi")),
  phiName_(dict.lookupOrDefault<word>("phi", "phi")),
  rhoName_(dict.lookupOrDefault<word>("rho", "rho"))
{}
mousse::mappedFlowRateFvPatchVectorField::mappedFlowRateFvPatchVectorField
(
  const mappedFlowRateFvPatchVectorField& ptf
)
:
  fixedValueFvPatchField<vector>(ptf),
  nbrPhiName_(ptf.nbrPhiName_),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_)
{}
mousse::mappedFlowRateFvPatchVectorField::mappedFlowRateFvPatchVectorField
(
  const mappedFlowRateFvPatchVectorField& ptf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchField<vector>(ptf, iF),
  nbrPhiName_(ptf.nbrPhiName_),
  phiName_(ptf.phiName_),
  rhoName_(ptf.rhoName_)
{}
// Member Functions 
void mousse::mappedFlowRateFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  // Since we're inside initEvaluate/evaluate there might be processor
  // comms underway. Change the tag we use.
  int oldTag = UPstream::msgType();
  UPstream::msgType() = oldTag+1;
  // Get the coupling information from the mappedPatchBase
  const mappedPatchBase& mpp = refCast<const mappedPatchBase>
  (
    patch().patch()
  );
  const polyMesh& nbrMesh = mpp.sampleMesh();
  const fvPatch& nbrPatch = refCast<const fvMesh>
  (
    nbrMesh
  ).boundary()[mpp.samplePolyPatch().index()];
  scalarList phi =
    nbrPatch.lookupPatchField<surfaceScalarField, scalar>(nbrPhiName_);
  mpp.distribute(phi);
  const surfaceScalarField& phiName =
    db().lookupObject<surfaceScalarField>(phiName_);
  scalarField U(-phi/patch().magSf());
  vectorField n(patch().nf());
  if (phiName.dimensions() == dimVelocity*dimArea)
  {
    // volumetric flow-rate
    operator==(n*U);
  }
  else if (phiName.dimensions() == dimDensity*dimVelocity*dimArea)
  {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    // mass flow-rate
    operator==(n*U/rhop);
    if (debug)
    {
      scalar phi = gSum(rhop*(*this) & patch().Sf());
      Info<< patch().boundaryMesh().mesh().name() << ':'
        << patch().name() << ':'
        << this->dimensionedInternalField().name() << " <- "
        << nbrMesh.name() << ':'
        << nbrPatch.name() << ':'
        << this->dimensionedInternalField().name() << " :"
        << " mass flux[Kg/s]:" << -phi
        << endl;
    }
  }
  else
  {
    FatalErrorIn
    (
      "mappedFlowRateFvPatchVectorField::updateCoeffs()"
    )   << "dimensions of " << phiName_ << " are incorrect" << nl
      << "    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << nl << exit(FatalError);
  }
  // Restore tag
  UPstream::msgType() = oldTag;
  fixedValueFvPatchField<vector>::updateCoeffs();
}
void mousse::mappedFlowRateFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchField<vector>::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  os.writeKeyword("nbrPhi") << nbrPhiName_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
namespace mousse
{
 makePatchTypeField
 (
   fvPatchVectorField,
   mappedFlowRateFvPatchVectorField
 );
}
