// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "convective_heat_transfer_fv_patch_scalar_field.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace compressible
{
// Constructors 
convectiveHeatTransferFvPatchScalarField::
convectiveHeatTransferFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(p, iF),
  L_(1.0)
{}
convectiveHeatTransferFvPatchScalarField::
convectiveHeatTransferFvPatchScalarField
(
  const convectiveHeatTransferFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField(ptf, p, iF, mapper),
  L_(ptf.L_)
{}
convectiveHeatTransferFvPatchScalarField::
convectiveHeatTransferFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField(p, iF, dict),
  L_(readScalar(dict.lookup("L")))
{}
convectiveHeatTransferFvPatchScalarField::
convectiveHeatTransferFvPatchScalarField
(
  const convectiveHeatTransferFvPatchScalarField& htcpsf
)
:
  fixedValueFvPatchScalarField(htcpsf),
  L_(htcpsf.L_)
{}
convectiveHeatTransferFvPatchScalarField::
convectiveHeatTransferFvPatchScalarField
(
  const convectiveHeatTransferFvPatchScalarField& htcpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField(htcpsf, iF),
  L_(htcpsf.L_)
{}
// Member Functions 
void convectiveHeatTransferFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const label patchi = patch().index();
  const compressible::turbulenceModel& turbModel =
    db().lookupObject<compressible::turbulenceModel>
    (
      IOobject::groupName
      (
        compressible::turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const scalarField alphaEffw(turbModel.alphaEff(patchi));
  const tmp<scalarField> tmuw = turbModel.mu(patchi);
  const scalarField& muw = tmuw();
  const scalarField& rhow = turbModel.rho().boundaryField()[patchi];
  const vectorField& Uc = turbModel.U();
  const vectorField& Uw = turbModel.U().boundaryField()[patchi];
  const scalarField& Tw = turbModel.transport().T().boundaryField()[patchi];
  const scalarField& pw = turbModel.transport().p().boundaryField()[patchi];
  const scalarField Cpw(turbModel.transport().Cp(pw, Tw, patchi));
  const scalarField kappaw(Cpw*alphaEffw);
  const scalarField Pr(muw*Cpw/kappaw);
  scalarField& htc = *this;
  forAll(htc, faceI)
  {
    label faceCellI = patch().faceCells()[faceI];
    scalar Re = rhow[faceI]*mag(Uc[faceCellI] - Uw[faceI])*L_/muw[faceI];
    if (Re < 5.0E+05)
    {
      htc[faceI] = 0.664*sqrt(Re)*cbrt(Pr[faceI])*kappaw[faceI]/L_;
    }
    else
    {
      htc[faceI] = 0.037*pow(Re, 0.8)*cbrt(Pr[faceI])*kappaw[faceI]/L_;
    }
  }
  fixedValueFvPatchScalarField::updateCoeffs();
}
// Member Functions 
void convectiveHeatTransferFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  os.writeKeyword("L") << L_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
makePatchTypeField
(
  fvPatchScalarField,
  convectiveHeatTransferFvPatchScalarField
);
}  // namespace compressible
}  // namespace mousse
