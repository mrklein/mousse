// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "johnson_jackson_particle_theta_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "two_phase_system.hpp"
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  JohnsonJacksonParticleThetaFvPatchScalarField
);
}
// Member Functions 
mousse::JohnsonJacksonParticleThetaFvPatchScalarField::
JohnsonJacksonParticleThetaFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{p, iF},
  restitutionCoefficient_{p.size()},
  specularityCoefficient_{p.size()}
{}
mousse::JohnsonJacksonParticleThetaFvPatchScalarField::
JohnsonJacksonParticleThetaFvPatchScalarField
(
  const JohnsonJacksonParticleThetaFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchScalarField{ptf, p, iF, mapper},
  restitutionCoefficient_{ptf.restitutionCoefficient_},
  specularityCoefficient_{ptf.specularityCoefficient_}
{
}
mousse::JohnsonJacksonParticleThetaFvPatchScalarField::
JohnsonJacksonParticleThetaFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchScalarField{p, iF},
  restitutionCoefficient_
  {
    "restitutionCoefficient",
    dimless,
    dict.lookup("restitutionCoefficient")
  },
  specularityCoefficient_
  {
    "specularityCoefficient",
    dimless,
    dict.lookup("specularityCoefficient")
  }
{
  if ((restitutionCoefficient_.value() < 0)
      || (restitutionCoefficient_.value() > 1))
  {
    FATAL_ERROR_IN
    (
      "mousse::JohnsonJacksonParticleThetaFvPatchScalarField::"
      "JohnsonJacksonParticleThetaFvPatchScalarField"
      "("
        "const fvPatch& p,"
        "const DimensionedField<scalar, volMesh>& iF,"
        "const dictionary& dict"
      ")"
    )
    << "The restitution coefficient has to be between 0 and 1"
    << abort(FatalError);
  }
  if ((specularityCoefficient_.value() < 0)
      || (specularityCoefficient_.value() > 1))
  {
    FATAL_ERROR_IN
    (
      "mousse::JohnsonJacksonParticleThetaFvPatchScalarField::"
      "JohnsonJacksonParticleThetaFvPatchScalarField"
      "("
        "const fvPatch& p,"
        "const DimensionedField<scalar, volMesh>& iF,"
        "const dictionary& dict"
      ")"
    )
    << "The specularity coefficient has to be between 0 and 1"
    << abort(FatalError);
  }
  fvPatchScalarField::operator=
  (
    scalarField("value", dict, p.size())
  );
}
mousse::JohnsonJacksonParticleThetaFvPatchScalarField::
JohnsonJacksonParticleThetaFvPatchScalarField
(
  const JohnsonJacksonParticleThetaFvPatchScalarField& ptf
)
:
  mixedFvPatchScalarField{ptf},
  restitutionCoefficient_{ptf.restitutionCoefficient_},
  specularityCoefficient_{ptf.specularityCoefficient_}
{}
mousse::JohnsonJacksonParticleThetaFvPatchScalarField::
JohnsonJacksonParticleThetaFvPatchScalarField
(
  const JohnsonJacksonParticleThetaFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  mixedFvPatchScalarField{ptf, iF},
  restitutionCoefficient_{ptf.restitutionCoefficient_},
  specularityCoefficient_{ptf.specularityCoefficient_}
{}
// Member Functions 
void mousse::JohnsonJacksonParticleThetaFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  mixedFvPatchScalarField::autoMap(m);
}
void mousse::JohnsonJacksonParticleThetaFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  mixedFvPatchScalarField::rmap(ptf, addr);
}
void mousse::JohnsonJacksonParticleThetaFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  // lookup the fluid model and the phase
  const twoPhaseSystem& fluid = db().lookupObject<twoPhaseSystem>
  (
    "phaseProperties"
  );
  const phaseModel& phased =
    fluid.phase1().name() == dimensionedInternalField().group()
    ? fluid.phase1()
    : fluid.phase2();
  // lookup all the fields on this patch
  const fvPatchScalarField& alpha =
    patch().lookupPatchField<volScalarField, scalar>
    (
      phased.volScalarField::name()
    );
  const fvPatchVectorField& U =
    patch().lookupPatchField<volVectorField, vector>
    (
      IOobject::groupName("U", phased.name())
    );
  const fvPatchScalarField& gs0 =
    patch().lookupPatchField<volScalarField, scalar>
    (
      IOobject::groupName("gs0", phased.name())
    );
  const fvPatchScalarField& kappa =
    patch().lookupPatchField<volScalarField, scalar>
    (
      IOobject::groupName("kappa", phased.name())
    );
  const scalarField Theta{patchInternalField()};
  // lookup the packed volume fraction
  dimensionedScalar alphaMax
  {
    "alphaMax",
    dimless,
    db().lookupObject<IOdictionary>
    (
      IOobject::groupName("turbulenceProperties", phased.name())
    )
    .subDict("RAS")
    .subDict("kineticTheoryCoeffs")
    .lookup("alphaMax")
  };
  // calculate the reference value and the value fraction
  if (restitutionCoefficient_.value() != 1.0)
  {
    this->refValue() =
      (2.0/3.0)
      *specularityCoefficient_.value()
      *magSqr(U)
      /(scalar(1) - sqr(restitutionCoefficient_.value()));
    this->refGrad() = 0.0;
    scalarField c
    {
      constant::mathematical::pi
      *alpha
      *gs0
      *(scalar(1) - sqr(restitutionCoefficient_.value()))
      *sqrt(3.0*Theta)
      /max(4.0*kappa*alphaMax.value(), SMALL)
    };
    this->valueFraction() = c/(c + patch().deltaCoeffs());
  }
  // for a restitution coefficient of 1, the boundary degenerates to a fixed
  // gradient condition
  else
  {
    this->refValue() = 0.0;
    this->refGrad() =
      pos(alpha - SMALL)
      *constant::mathematical::pi
      *specularityCoefficient_.value()
      *alpha
      *gs0
      *sqrt(3.0*Theta)
      *magSqr(U)
      /max(6.0*kappa*alphaMax.value(), SMALL);
    this->valueFraction() = 0.0;
  }
  mixedFvPatchScalarField::updateCoeffs();
}
void mousse::JohnsonJacksonParticleThetaFvPatchScalarField::write
(
  Ostream& os
) const
{
  fvPatchScalarField::write(os);
  os.writeKeyword("restitutionCoefficient")
    << restitutionCoefficient_ << token::END_STATEMENT << nl;
  os.writeKeyword("specularityCoefficient")
    << specularityCoefficient_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
