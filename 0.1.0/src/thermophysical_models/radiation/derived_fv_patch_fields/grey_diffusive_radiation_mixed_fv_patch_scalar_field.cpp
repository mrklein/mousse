// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "grey_diffusive_radiation_mixed_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "fv_dom.hpp"
#include "constants.hpp"
using namespace mousse::constant;
using namespace mousse::constant::mathematical;
// Constructors 
mousse::radiation::greyDiffusiveRadiationMixedFvPatchScalarField::
greyDiffusiveRadiationMixedFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField(p, iF),
  radiationCoupledBase(p, "undefined", scalarField::null()),
  TName_("T")
{
  refValue() = 0.0;
  refGrad() = 0.0;
  valueFraction() = 1.0;
}
mousse::radiation::greyDiffusiveRadiationMixedFvPatchScalarField::
greyDiffusiveRadiationMixedFvPatchScalarField
(
  const greyDiffusiveRadiationMixedFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField(ptf, p, iF, mapper),
  radiationCoupledBase
  (
    p,
    ptf.emissivityMethod(),
    ptf.emissivity_
  ),
  TName_(ptf.TName_)
{}
mousse::radiation::greyDiffusiveRadiationMixedFvPatchScalarField::
greyDiffusiveRadiationMixedFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField(p, iF),
  radiationCoupledBase(p, dict),
  TName_(dict.lookupOrDefault<word>("T", "T"))
{
  if (dict.found("refValue"))
  {
    fvPatchScalarField::operator=
    (
      scalarField("value", dict, p.size())
    );
    refValue() = scalarField("refValue", dict, p.size());
    refGrad() = scalarField("refGradient", dict, p.size());
    valueFraction() = scalarField("valueFraction", dict, p.size());
  }
  else
  {
    refValue() = 0.0;
    refGrad() = 0.0;
    valueFraction() = 1.0;
    fvPatchScalarField::operator=(refValue());
  }
}
mousse::radiation::greyDiffusiveRadiationMixedFvPatchScalarField::
greyDiffusiveRadiationMixedFvPatchScalarField
(
  const greyDiffusiveRadiationMixedFvPatchScalarField& ptf
)
:
  mixedFvPatchScalarField(ptf),
  radiationCoupledBase
  (
    ptf.patch(),
    ptf.emissivityMethod(),
    ptf.emissivity_
  ),
  TName_(ptf.TName_)
{}
mousse::radiation::greyDiffusiveRadiationMixedFvPatchScalarField::
greyDiffusiveRadiationMixedFvPatchScalarField
(
  const greyDiffusiveRadiationMixedFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField(ptf, iF),
  radiationCoupledBase
  (
    ptf.patch(),
    ptf.emissivityMethod(),
    ptf.emissivity_
  ),
  TName_(ptf.TName_)
{}
// Member Functions 
void mousse::radiation::greyDiffusiveRadiationMixedFvPatchScalarField::
updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  // Since we're inside initEvaluate/evaluate there might be processor
  // comms underway. Change the tag we use.
  int oldTag = UPstream::msgType();
  UPstream::msgType() = oldTag+1;
  const scalarField& Tp =
    patch().lookupPatchField<volScalarField, scalar>(TName_);
  const radiationModel& radiation =
    db().lookupObject<radiationModel>("radiationProperties");
  const fvDOM& dom(refCast<const fvDOM>(radiation));
  label rayId = -1;
  label lambdaId = -1;
  dom.setRayIdLambdaId(dimensionedInternalField().name(), rayId, lambdaId);
  const label patchI = patch().index();
  if (dom.nLambda() != 1)
  {
    FATAL_ERROR_IN
    (
      "mousse::radiation::"
      "greyDiffusiveRadiationMixedFvPatchScalarField::updateCoeffs"
    )   << " a grey boundary condition is used with a non-grey "
      << "absorption model" << nl << exit(FatalError);
  }
  scalarField& Iw = *this;
  const vectorField n(patch().nf());
  radiativeIntensityRay& ray =
    const_cast<radiativeIntensityRay&>(dom.IRay(rayId));
  const scalarField nAve(n & ray.dAve());
  ray.Qr().boundaryField()[patchI] += Iw*nAve;
  const scalarField temissivity = emissivity();
  scalarField& Qem = ray.Qem().boundaryField()[patchI];
  scalarField& Qin = ray.Qin().boundaryField()[patchI];
  const vector& myRayId = dom.IRay(rayId).d();
  // Use updated Ir while iterating over rays
  // avoids to used lagged Qin
  scalarField Ir = dom.IRay(0).Qin().boundaryField()[patchI];
  for (label rayI=1; rayI < dom.nRay(); rayI++)
  {
    Ir += dom.IRay(rayI).Qin().boundaryField()[patchI];
  }
  FOR_ALL(Iw, faceI)
  {
    if ((-n[faceI] & myRayId) > 0.0)
    {
      // direction out of the wall
      refGrad()[faceI] = 0.0;
      valueFraction()[faceI] = 1.0;
      refValue()[faceI] =
        (
          Ir[faceI]*(scalar(1.0) - temissivity[faceI])
         + temissivity[faceI]*physicoChemical::sigma.value()
         * pow4(Tp[faceI])
        )/pi;
      // Emmited heat flux from this ray direction
      Qem[faceI] = refValue()[faceI]*nAve[faceI];
    }
    else
    {
      // direction into the wall
      valueFraction()[faceI] = 0.0;
      refGrad()[faceI] = 0.0;
      refValue()[faceI] = 0.0; //not used
      // Incident heat flux on this ray direction
      Qin[faceI] = Iw[faceI]*nAve[faceI];
    }
  }
  // Restore tag
  UPstream::msgType() = oldTag;
  mixedFvPatchScalarField::updateCoeffs();
}
void mousse::radiation::greyDiffusiveRadiationMixedFvPatchScalarField::write
(
  Ostream& os
) const
{
  mixedFvPatchScalarField::write(os);
  radiationCoupledBase::write(os);
  writeEntryIfDifferent<word>(os, "T", "T", TName_);
}
namespace mousse
{
namespace radiation
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  greyDiffusiveRadiationMixedFvPatchScalarField
);
}
}
