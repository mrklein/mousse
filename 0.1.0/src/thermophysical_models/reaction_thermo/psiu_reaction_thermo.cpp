// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psiu_reaction_thermo.hpp"
#include "fv_mesh.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "fixed_unburnt_enthalpy_fv_patch_scalar_field.hpp"
#include "gradient_unburnt_enthalpy_fv_patch_scalar_field.hpp"
#include "mixed_unburnt_enthalpy_fv_patch_scalar_field.hpp"
namespace mousse
{
/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */
DEFINE_TYPE_NAME_AND_DEBUG(psiuReactionThermo, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(psiuReactionThermo, fvMesh);
// Private Member Functions 
wordList psiuReactionThermo::heuBoundaryTypes()
{
  const volScalarField::GeometricBoundaryField& tbf =
    this->Tu().boundaryField();
  wordList hbt = tbf.types();
  FOR_ALL(tbf, patchi)
  {
    if (isA<fixedValueFvPatchScalarField>(tbf[patchi]))
    {
      hbt[patchi] = fixedUnburntEnthalpyFvPatchScalarField::typeName;
    }
    else if
    (
      isA<zeroGradientFvPatchScalarField>(tbf[patchi])
    || isA<fixedGradientFvPatchScalarField>(tbf[patchi])
    )
    {
      hbt[patchi] = gradientUnburntEnthalpyFvPatchScalarField::typeName;
    }
    else if (isA<mixedFvPatchScalarField>(tbf[patchi]))
    {
      hbt[patchi] = mixedUnburntEnthalpyFvPatchScalarField::typeName;
    }
  }
  return hbt;
}
void psiuReactionThermo::heuBoundaryCorrection(volScalarField& heu)
{
  volScalarField::GeometricBoundaryField& hbf = heu.boundaryField();
  FOR_ALL(hbf, patchi)
  {
    if
    (
      isA<gradientUnburntEnthalpyFvPatchScalarField>(hbf[patchi])
    )
    {
      refCast<gradientUnburntEnthalpyFvPatchScalarField>(hbf[patchi])
        .gradient() = hbf[patchi].fvPatchField::snGrad();
    }
    else if
    (
      isA<mixedUnburntEnthalpyFvPatchScalarField>(hbf[patchi])
    )
    {
      refCast<mixedUnburntEnthalpyFvPatchScalarField>(hbf[patchi])
        .refGrad() = hbf[patchi].fvPatchField::snGrad();
    }
  }
}
// Constructors 
psiuReactionThermo::psiuReactionThermo
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  psiReactionThermo(mesh, phaseName)
{}
// Selectors
mousse::autoPtr<mousse::psiuReactionThermo> mousse::psiuReactionThermo::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicThermo::New<psiuReactionThermo>(mesh, phaseName);
}
// Destructor 
psiuReactionThermo::~psiuReactionThermo()
{}
}  // namespace mousse
