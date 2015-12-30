// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "temperature_coupled_base.hpp"
#include "vol_fields.hpp"
#include "fluid_thermo.hpp"
#include "solid_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
// Static Member Data 
namespace mousse
{
  template<>
  const char* mousse::NamedEnum
  <
    mousse::temperatureCoupledBase::KMethodType,
    4
  >::names[] =
  {
    "fluidThermo",
    "solidThermo",
    "directionalSolidThermo",
    "lookup"
  };
}
const mousse::NamedEnum<mousse::temperatureCoupledBase::KMethodType, 4>
  mousse::temperatureCoupledBase::KMethodTypeNames_;
// Constructors 
mousse::temperatureCoupledBase::temperatureCoupledBase
(
  const fvPatch& patch,
  const word& calculationType,
  const word& kappaName,
  const word& alphaAniName
)
:
  patch_(patch),
  method_(KMethodTypeNames_[calculationType]),
  kappaName_(kappaName),
  alphaAniName_(alphaAniName)
{}
mousse::temperatureCoupledBase::temperatureCoupledBase
(
  const fvPatch& patch,
  const dictionary& dict
)
:
  patch_(patch),
  method_(KMethodTypeNames_.read(dict.lookup("kappa"))),
  kappaName_(dict.lookup("kappaName")),
  alphaAniName_(dict.lookupOrDefault<word>("alphaAniName","Anialpha"))
{}
mousse::temperatureCoupledBase::temperatureCoupledBase
(
  const fvPatch& patch,
  const temperatureCoupledBase& base
)
:
  patch_(patch),
  method_(base.method_),
  kappaName_(base.kappaName_),
  alphaAniName_(base.alphaAniName_)
{}
// Member Functions 
mousse::tmp<mousse::scalarField> mousse::temperatureCoupledBase::kappa
(
  const scalarField& Tp
) const
{
  const fvMesh& mesh = patch_.boundaryMesh().mesh();
  const label patchI = patch_.index();
  switch (method_)
  {
    case mtFluidThermo:
    {
      typedef compressible::turbulenceModel turbulenceModel;
      word turbName(turbulenceModel::propertiesName);
      if
      (
        mesh.foundObject<turbulenceModel>(turbName)
      )
      {
        const turbulenceModel& turbModel =
          mesh.lookupObject<turbulenceModel>(turbName);
        return turbModel.kappaEff(patchI);
      }
      else if (mesh.foundObject<fluidThermo>(basicThermo::dictName))
      {
        const fluidThermo& thermo =
          mesh.lookupObject<fluidThermo>(basicThermo::dictName);
        return thermo.kappa(patchI);
      }
      else
      {
        FatalErrorIn
        (
          "temperatureCoupledBase::kappa(const scalarField&) const"
        )
          << "Kappa defined to employ " << KMethodTypeNames_[method_]
          << " method, but thermo package not available"
          << exit(FatalError);
      }
      break;
    }
    case mtSolidThermo:
    {
      const solidThermo& thermo =
        mesh.lookupObject<solidThermo>(basicThermo::dictName);
      return thermo.kappa(patchI);
      break;
    }
    case mtDirectionalSolidThermo:
    {
      const solidThermo& thermo =
        mesh.lookupObject<solidThermo>(basicThermo::dictName);
      const symmTensorField& alphaAni =
        patch_.lookupPatchField<volSymmTensorField, scalar>
        (
          alphaAniName_
        );
      const scalarField& pp = thermo.p().boundaryField()[patchI];
      const symmTensorField kappa(alphaAni*thermo.Cp(pp, Tp, patchI));
      const vectorField n(patch_.nf());
      return n & kappa & n;
    }
    case mtLookup:
    {
      if (mesh.foundObject<volScalarField>(kappaName_))
      {
        return patch_.lookupPatchField<volScalarField, scalar>
        (
          kappaName_
        );
      }
      else if (mesh.foundObject<volSymmTensorField>(kappaName_))
      {
        const symmTensorField& KWall =
          patch_.lookupPatchField<volSymmTensorField, scalar>
          (
            kappaName_
          );
        const vectorField n(patch_.nf());
        return n & KWall & n;
      }
      else
      {
        FatalErrorIn
        (
          "temperatureCoupledBase::kappa(const scalarField&) const"
        )
          << "Did not find field " << kappaName_
          << " on mesh " << mesh.name() << " patch " << patch_.name()
          << nl
          << "Please set 'kappa' to one of "
          << KMethodTypeNames_.toc()
          << " and 'kappaName' to the name of the volScalar"
          << " or volSymmTensor field (if kappa=lookup)"
          << exit(FatalError);
      }
      break;
    }
    default:
    {
      FatalErrorIn
      (
        "temperatureCoupledBase::kappa(const scalarField&) const"
      )
        << "Unimplemented method " << KMethodTypeNames_[method_] << nl
        << "Please set 'kappa' to one of " << KMethodTypeNames_.toc()
        << " and 'kappaName' to the name of the volScalar"
        << " or volSymmTensor field (if kappa=lookup)"
        << exit(FatalError);
    }
  }
  return scalarField(0);
}
void mousse::temperatureCoupledBase::write(Ostream& os) const
{
  os.writeKeyword("kappa") << KMethodTypeNames_[method_]
    << token::END_STATEMENT << nl;
  os.writeKeyword("kappaName") << kappaName_ << token::END_STATEMENT << nl;
}
